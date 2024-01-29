#include "../common/Layout.hpp"
#include "settings-widgets/DirectorySelector.hpp"
#include "settings-widgets/FontSelector.h"
#include "WindowSettings.h"

#include <QApplication>
#include <QByteArray>
#include <QDockWidget>
#include <QGridLayout>
#include <QRect>
#include <QStyle>
//#include <QTextOption>

constexpr char DATA[] = "Data";
constexpr char DATA_PROJECTS[] = "Projects path";

constexpr char EDITOR[] = "Editor";
constexpr char EDITOR_FONT[] = "Font";
//constexpr char EDITOR_TAB_STOP[] = "Tab stop distance (px)";
constexpr char EDITOR_TYPEWRITER[] = "Typewriter";
//constexpr char EDITOR_WRAP[] = "Wrap";

constexpr char METER[] = "Meter";
constexpr char METER_LINE_POS[] = "Line position";
constexpr char METER_COL_POS[] = "Column position";
constexpr char METER_LINES[] = "Line count";
constexpr char METER_WORDS[] = "Word count";
constexpr char METER_CHARS[] = "Character count";

constexpr char WINDOW[] = "Window";
constexpr char WINDOW_DOCK_POS[] = "Dock position";
constexpr char WINDOW_DOCK_VIS[] = "Dock visibile";
constexpr char WINDOW_GEOMETRY[] = "Geometry";

constexpr char INI_FILLER[] = "_";

constexpr char EDITOR_FONT_DEFAULT[] = "mononoki";
constexpr int EDITOR_FONTSIZE_DEFAULT = 12;
constexpr char DATA_PROJECTS_DEFAULT[] = "Fernanda";

WindowSettings::WindowSettings(const Path& config, QObject* parent)
	: QObject(parent), m_iniWriter(new IniWriter(config, this))
{
	loadAll();
}

WindowSettings::~WindowSettings()
{
	qDebug() << __FUNCTION__;

	saveAll();
}

void WindowSettings::yoke(Window* window)
{
	m_windows << window;

	applyAll(window);
	window->installEventFilter(this);

	connect(window->m_dockWidget, &QDockWidget::dockLocationChanged, this, [&](Qt::DockWidgetArea area) {
		passiveApply(WINDOW, WINDOW_DOCK_POS, static_cast<int>(area));
		});
	connect(window->m_dockWidget, &QDockWidget::visibilityChanged, this, [=](bool visible) {
		if (window->isVisible())
			passiveApply<bool>(WINDOW, WINDOW_DOCK_VIS, visible);
		});
}

void WindowSettings::detach(Window* window)
{
	m_windows.removeAll(window);

	window->removeEventFilter(this);
}

void WindowSettings::openDialog()
{
	if (m_dialog) {
		m_dialog->raise();
		m_dialog->activateWindow();

		return;
	}

	m_dialog = new QDialog;
	setupDialog(m_dialog);

	m_dialog->open();
}

bool WindowSettings::eventFilter(QObject* watched, QEvent* event)
{
	if (event->type() == QEvent::Resize || event->type() == QEvent::Move)
	{
		auto window = qobject_cast<Window*>(watched);
		passiveApply<QByteArray>(WINDOW, WINDOW_GEOMETRY, window->saveGeometry());

		return true;
	}

	return false;
}

void WindowSettings::loadAll()
{
	loadDataSettings();
	loadEditorSettings();
	loadMeterSettings();
	loadWindowSettings();
}

void WindowSettings::loadDataSettings()
{
	m_iniWriter->begin(DATA);
	QMap<QString, Setting> data_settings;

	auto default_projects_path = Path::system(Path::System::Documents) / DATA_PROJECTS_DEFAULT;

	data_settings[DATA_PROJECTS] = {
		m_iniWriter->load(iniName(DATA_PROJECTS), default_projects_path),

		[setting = &data_settings[DATA_PROJECTS], default_projects_path](Window* window) {
			auto dir = setting->value<Path>();

			if (!dir.isValid()) {
				setting->variant = default_projects_path;
				dir = default_projects_path;
			}

			if (dir == default_projects_path && !dir.isValid())
				Path::createDirs(dir);

			window->m_treeView->setRoot(dir);
		}
	};

	//...

	m_settings[DATA] = data_settings;
	m_iniWriter->end();
}

void WindowSettings::loadEditorSettings()
{
	m_iniWriter->begin(EDITOR);
	QMap<QString, Setting> editor_settings;

	auto default_font = QFont(EDITOR_FONT_DEFAULT, EDITOR_FONTSIZE_DEFAULT);

	editor_settings[EDITOR_FONT] = {
		m_iniWriter->load(iniName(EDITOR_FONT), default_font),

		[setting = &editor_settings[EDITOR_FONT], default_font](Window* window) {
			auto font = setting->value<QFont>();
			auto point_size = font.pointSize();

			if (!font.exactMatch() && (point_size < 6 || point_size > 144)) {
				setting->variant = default_font;
				font = default_font;
			}

			window->m_editorsFont = font;

			for (auto& editor : window->editors())
				editor->setFont(font);
		}
	};

	editor_settings[EDITOR_TYPEWRITER] = {
		m_iniWriter->load(iniName(EDITOR_TYPEWRITER), false),

		[setting = &editor_settings[EDITOR_TYPEWRITER]](Window* window) {
			auto is_typewriter = setting->value<bool>();

			window->m_editorsIsTypewriter = is_typewriter;

			for (auto& editor : window->editors())
				editor->setIsTypewriter(is_typewriter);
		}
	};

	m_settings[EDITOR] = editor_settings;
	m_iniWriter->end();
}

void WindowSettings::loadMeterSettings()
{
	m_iniWriter->begin(METER);
	QMap<QString, Setting> meter_settings;

	meter_settings[METER_LINE_POS] = {
		m_iniWriter->load(iniName(METER_LINE_POS), true),

		[setting = &meter_settings[METER_LINE_POS]](Window* window) {
			window->m_meter->setHasLinePosition(setting->value<bool>());
		}
	};

	meter_settings[METER_COL_POS] = {
		m_iniWriter->load(iniName(METER_COL_POS), true),

		[setting = &meter_settings[METER_COL_POS]](Window* window) {
			window->m_meter->setHasColumnPosition(setting->value<bool>());
		}
	};

	meter_settings[METER_LINES] = {
		m_iniWriter->load(iniName(METER_LINES), false),

		[setting = &meter_settings[METER_LINES]](Window* window) {
			window->m_meter->setHasLineCount(setting->value<bool>());
		}
	};

	meter_settings[METER_WORDS] = {
		m_iniWriter->load(iniName(METER_WORDS), false),

		[setting = &meter_settings[METER_WORDS]](Window* window) {
			window->m_meter->setHasWordCount(setting->value<bool>());
		}
	};

	meter_settings[METER_CHARS] = {
		m_iniWriter->load(iniName(METER_CHARS), false),

		[setting = &meter_settings[METER_CHARS]](Window* window) {
			window->m_meter->setHasCharCount(setting->value<bool>());
		}
	};

	m_settings[METER] = meter_settings;
	m_iniWriter->end();
}

void WindowSettings::loadWindowSettings()
{
	m_iniWriter->begin(WINDOW);
	QMap<QString, Setting> window_settings;

	auto dock_pos_fallback = static_cast<int>(Qt::LeftDockWidgetArea);

	window_settings[WINDOW_DOCK_POS] = {
		m_iniWriter->load(iniName(WINDOW_DOCK_POS), dock_pos_fallback),

		[setting = &window_settings[WINDOW_DOCK_POS], dock_pos_fallback](Window* window) {
			auto area = static_cast<Qt::DockWidgetArea>(setting->value<int>());

			if (area == Qt::NoDockWidgetArea) {
				setting->variant = dock_pos_fallback;
				area = static_cast<Qt::DockWidgetArea>(dock_pos_fallback);
			}

			window->addDockWidget(area, window->m_dockWidget);
		}
	};

	window_settings[WINDOW_DOCK_VIS] = {
		m_iniWriter->load(iniName(WINDOW_DOCK_VIS), true),

		[setting = &window_settings[WINDOW_DOCK_VIS]](Window* window) {
			window->m_dockWidget->setVisible(setting->value<bool>());
		}
	};

	window_settings[WINDOW_GEOMETRY] = {
		m_iniWriter->load(iniName(WINDOW_GEOMETRY)),

		[setting = &window_settings[WINDOW_GEOMETRY], this](Window* window) {
			auto byte_array = setting->value<QByteArray>();

			if (!window->restoreGeometry(byte_array))
				window->setGeometry(QRect(0, 0, 1000, 600));

			moveXYIfTaken(window);
		}
	};

	m_settings[WINDOW] = window_settings;
	m_iniWriter->end();
}

void WindowSettings::saveAll()
{
	saveSettings(DATA, { DATA_PROJECTS });
	saveSettings(EDITOR, { EDITOR_FONT, EDITOR_TYPEWRITER });
	saveSettings(METER, { METER_LINE_POS, METER_COL_POS, METER_LINES, METER_WORDS, METER_CHARS });
	saveSettings(WINDOW, { WINDOW_DOCK_POS, WINDOW_DOCK_VIS, WINDOW_GEOMETRY });
}

void WindowSettings::saveSetting(const QString& prefix, const QString& key, AppendPrefix usePrefix)
{
	auto use_prefix = (usePrefix == AppendPrefix::Yes);

	if (use_prefix)
		m_iniWriter->begin(prefix);

	m_iniWriter->save(iniName(key), variantAt(prefix, key));

	if (use_prefix)
		m_iniWriter->end();
}

void WindowSettings::saveSettings(const QString& prefix, QStringList keys)
{
	m_iniWriter->begin(prefix);

	for (auto& key : keys)
		saveSetting(prefix, key, AppendPrefix::No);

	m_iniWriter->end();
}

QString WindowSettings::iniName(QString text)
{
	return text.replace(" ", INI_FILLER);
}

void WindowSettings::setupDialog(QDialog* dialog)
{
	auto grid = new QGridLayout(dialog);

	dialog->setLayout(grid);
	dialog->setFixedSize(800, 500);

	auto margins = QMargins(5, 5, 5, 5);
	auto spacing = 5;

	grid->addWidget(newDataBox(dialog, margins, spacing));
	grid->addWidget(newEditorBox(dialog, margins, spacing));
	grid->addWidget(newMeterBox(dialog, margins, spacing));

	grid->setContentsMargins(margins);
	grid->setSpacing(spacing);

	connect(m_dialog, &QDialog::finished, this, &WindowSettings::onDialogFinished);

	// v--- Filler

	/*for (auto i = 0; i < 2; ++i) {
		auto group_box = new QGroupBox(dialog);
		auto combo_box = new QComboBox(group_box);
		group_box->setTitle(QString("Group Box %1").arg(i + 1));
		Layout::box(Layout::Box::Horizontal, group_box, QWidgetList{ combo_box });
		grid->addWidget(group_box);
	}*/
}

QGroupBox* WindowSettings::newGroupBox(QDialog* dialog, QLayout* layout, const QMargins& margins, int spacing)
{
	auto box = new QGroupBox(dialog);

	box->setContentsMargins(margins);
	layout->setContentsMargins(margins);
	layout->setSpacing(spacing);

	box->setLayout(layout);

	return box;
}

QGroupBox* WindowSettings::newDataBox(QDialog* dialog, const QMargins& margins, int spacing)
{
	auto initial_dir = valueAt<Path>(DATA, DATA_PROJECTS);
	auto dir_selector = new DirectorySelector(dialog, initial_dir);

	connect(dir_selector, &DirectorySelector::selected, this, [&](const Path& directory) {
		activeApply<Path>(DATA, DATA_PROJECTS, directory);
		});

	auto layout = Layout::box(Layout::Box::Horizontal, QWidgetList{ dir_selector });

	auto box = newGroupBox(dialog, layout, margins, spacing);
	box->setTitle(DATA_PROJECTS);

	return box;
}

QGroupBox* WindowSettings::newEditorBox(QDialog* dialog, const QMargins& margins, int spacing)
{
	QWidgetList check_boxes = {
		newCheckBox(EDITOR, EDITOR_TYPEWRITER, dialog)
	};

	auto bottom_layout = Layout::box(Layout::Box::Horizontal, check_boxes);

	QObjectList objects = {
		newFontBox(dialog, margins, spacing),
		bottom_layout
	};

	auto layout = Layout::box(Layout::Box::Vertical, objects);

	auto box = newGroupBox(dialog, layout, margins, spacing);
	box->setTitle(EDITOR);

	return box;
}

QGroupBox* WindowSettings::newFontBox(QDialog* dialog, const QMargins& margins, int spacing)
{
	auto initial_font = valueAt<QFont>(EDITOR, EDITOR_FONT);
	auto font_selector = new FontSelector(dialog, initial_font);

	connect(font_selector, &FontSelector::currentFontChanged, this, [&](const QFont& font) {
		activeApply<QFont>(EDITOR, EDITOR_FONT, font);
		});

	auto layout = Layout::box(Layout::Box::Horizontal, QWidgetList{ font_selector });
	// ^ Overload to take 1 widget/object

	auto box = newGroupBox(dialog, layout, margins, spacing);
	box->setTitle(EDITOR_FONT);

	return box;
}

QGroupBox* WindowSettings::newMeterBox(QDialog* dialog, const QMargins& margins, int spacing)
{
	// Toggle/untoggle all button.

	QWidgetList check_boxes = {
		newCheckBox(METER, METER_LINE_POS, dialog),
		newCheckBox(METER, METER_COL_POS, dialog),
		newCheckBox(METER, METER_LINES, dialog),
		newCheckBox(METER, METER_WORDS, dialog),
		newCheckBox(METER, METER_CHARS, dialog)
	};

	auto layout = Layout::box(Layout::Box::Horizontal, check_boxes);

	auto box = newGroupBox(dialog, layout, margins, spacing);
	box->setTitle(EDITOR_FONT);

	return box;
}

QCheckBox* WindowSettings::newCheckBox(const QString& prefix, const QString& key, QWidget* parent)
{
	auto check_box = new QCheckBox(key, parent);

	check_box->setTristate(false);
	check_box->setChecked(valueAt<bool>(prefix, key));

	connect(check_box, &QCheckBox::stateChanged, this, [=](int state) {
		activeApply<bool>(prefix, key, state);
		});

	return check_box;
}

void WindowSettings::moveXYIfTaken(Window* window)
{
	for (auto& other : m_windows) {
		if (other == window) continue;

		auto x = window->x();
		auto y = window->y();

		auto distance = 6;
		auto title_bar_height = QApplication::style()->pixelMetric(QStyle::PM_TitleBarHeight);

		if (x == other->x() /*&& y == other->y()*/)
			window->move(x + distance + title_bar_height, y + distance);
	}
}

void WindowSettings::syncUp(const QString& prefix, const QString& key)
{
	for (auto& window : m_windows)
		m_settings[prefix][key].action(window);
}

void WindowSettings::applyAll(Window* window)
{
	for (auto it = m_settings.begin(); it != m_settings.end(); ++it)
		for (auto sub_it = it.value().begin(); sub_it != it.value().end(); ++sub_it)
			sub_it.value().action(window);
}

QVariant WindowSettings::variantAt(const QString& prefix, const QString& key)
{
	return m_settings[prefix][key].variant;
}

void WindowSettings::onDialogFinished()
{
	delete m_dialog;

	m_dialog = nullptr;
}
