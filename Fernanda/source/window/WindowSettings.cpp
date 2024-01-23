#include "../common/Layout.hpp"
#include "settings-widgets/DirectorySelector.hpp"
#include "settings-widgets/FontSelector.h"
#include "WindowSettings.h"

#include <QApplication>
#include <QByteArray>
#include <QFileDialog>
#include <QGridLayout>
#include <QRect>
#include <QStyle>

constexpr char INI_FILLER[] = "_";
constexpr char DATA[] = "Data";
constexpr char DATA_PROJECTS[] = "Projects path";
constexpr char DATA_PROJECTS_DEFAULT[] = "Fernanda";
constexpr char EDITOR[] = "Editor";
constexpr char EDITOR_FONT[] = "Font";
constexpr char EDITOR_FONT_DEFAULT[] = "mononoki";
constexpr int EDITOR_FONTSIZE_DEFAULT = 12;
constexpr char METER[] = "Meter";
constexpr char METER_LINE_POS[] = "Line position";
constexpr char METER_COL_POS[] = "Column position";
constexpr char METER_LINES[] = "Line count";
constexpr char METER_WORDS[] = "Word count";
constexpr char METER_CHARS[] = "Character count";
constexpr char WINDOW[] = "Window";
constexpr char WINDOW_GEOMETRY[] = "Geometry";

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

	// If saved dir doesn't exist, switch to default, and if default doesn't exist, make it

	data_settings[DATA_PROJECTS] = {
		m_iniWriter->load(iniName(DATA_PROJECTS), default_projects_path),

		[setting = &data_settings[DATA_PROJECTS]](Window* window) {
			window->m_treeView->setRoot(setting->value<Path>());
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

			if (!font.exactMatch() && (point_size < 6 || point_size > 144))
				setting->variant = default_font;

			window->m_editorFont = setting->value<QFont>();

			for (auto& editor : window->editors())
				if (editor)
					editor->setFont(window->m_editorFont);
		}
	};

	//...

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
	saveDataSettings();
	saveEditorSettings();
	saveMeterSettings();
	saveWindowSettings();
}

void WindowSettings::saveDataSettings()
{
	m_iniWriter->begin(DATA);

	m_iniWriter->save(iniName(DATA_PROJECTS), variantAt(DATA, DATA_PROJECTS));
	//...

	m_iniWriter->end();
}

void WindowSettings::saveEditorSettings()
{
	m_iniWriter->begin(EDITOR);

	m_iniWriter->save(iniName(EDITOR_FONT), variantAt(EDITOR, EDITOR_FONT));
	//...

	m_iniWriter->end();
}

void WindowSettings::saveMeterSettings()
{
	m_iniWriter->begin(METER);

	m_iniWriter->save(iniName(METER_LINE_POS), variantAt(METER, METER_LINE_POS));
	m_iniWriter->save(iniName(METER_COL_POS), variantAt(METER, METER_COL_POS));
	m_iniWriter->save(iniName(METER_LINES), variantAt(METER, METER_LINES));
	m_iniWriter->save(iniName(METER_WORDS), variantAt(METER, METER_WORDS));
	m_iniWriter->save(iniName(METER_CHARS), variantAt(METER, METER_CHARS));

	m_iniWriter->end();
}

void WindowSettings::saveWindowSettings()
{
	m_iniWriter->begin(WINDOW);

	m_iniWriter->save(iniName(WINDOW_GEOMETRY), m_settings[WINDOW][WINDOW_GEOMETRY].variant);
	//...

	m_iniWriter->end();
}

QString WindowSettings::iniName(QString text)
{
	return text.replace(" ", INI_FILLER);
}

void WindowSettings::setupDialog(QDialog* dialog)
{
	// Set uniform sizing for all of these QGroupBoxes

	auto grid = new QGridLayout(dialog);
	auto space = 5;
	//grid->setContentsMargins(space, space, space, space);
	//grid->setSpacing(space);

	dialog->setLayout(grid);
	dialog->setFixedSize(800, 500);

	grid->addWidget(fontBox(dialog));
	grid->addWidget(meterBox(dialog));
	grid->addWidget(defaultProjectPathBox(dialog));

	grid->setContentsMargins(space, space, space, space);
	grid->setSpacing(space);

	connect(m_dialog, &QDialog::finished, this, &WindowSettings::onDialogFinished);

	// v--- Filler

	for (auto i = 0; i < 2; ++i) {
		auto group_box = new QGroupBox(dialog);
		auto combo_box = new QComboBox(group_box);
		group_box->setTitle(QString("Group Box %1").arg(i + 1));
		Layout::box(Layout::Box::Horizontal, group_box, QWidgetList{ combo_box });
		grid->addWidget(group_box);
	}
}

QGroupBox* WindowSettings::fontBox(QDialog* dialog)
{
	auto box = new QGroupBox(dialog);
	box->setTitle(EDITOR_FONT);

	auto initial_font = valueAt<QFont>(EDITOR, EDITOR_FONT);
	auto font_selector = new FontSelector(box, initial_font);
	
	Layout::box(Layout::Box::Horizontal, box, QWidgetList{ font_selector }); // Overload to take 1 widget/object

	connect(font_selector, &FontSelector::currentFontChanged, this, [&](const QFont& font) {
		activeApply<QFont>(EDITOR, EDITOR_FONT, font);
		});

	return box;
}

QGroupBox* WindowSettings::meterBox(QDialog* dialog)
{
	auto box = new QGroupBox(dialog);
	box->setTitle(METER);

	QWidgetList check_boxes = {
		newCheckBox(METER, METER_LINE_POS, box),
		newCheckBox(METER, METER_COL_POS, box),
		newCheckBox(METER, METER_LINES, box),
		newCheckBox(METER, METER_WORDS, box),
		newCheckBox(METER, METER_CHARS, box)
	};

	//auto space = 5;
	//box->setContentsMargins(space, space, space, space);
	auto layout = Layout::box(Layout::Box::Horizontal, box, check_boxes);
	//layout->setContentsMargins(space, space, space, space);

	return box;
}

QGroupBox* WindowSettings::defaultProjectPathBox(QDialog* dialog)
{
	auto box = new QGroupBox(dialog);
	box->setTitle(DATA_PROJECTS);

	auto initial_dir = valueAt<Path>(DATA, DATA_PROJECTS);
	auto dir_selector = new DirectorySelector(dialog, initial_dir);

	auto layout = Layout::box(Layout::Box::Horizontal, box, QWidgetList{ dir_selector });
	//layout->setContentsMargins(5, 5, 5, 5);

	connect(dir_selector, &DirectorySelector::selected, this, [&](const Path& directory) {
		activeApply<Path>(DATA, DATA_PROJECTS, directory);
		});

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

		auto d = 6;
		auto title_bar_height = QApplication::style()->pixelMetric(QStyle::PM_TitleBarHeight);

		if (x == other->x() /*&& y == other->y()*/)
			window->move(x + d + title_bar_height, y + d);
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
