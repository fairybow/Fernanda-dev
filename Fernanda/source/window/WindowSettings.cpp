#include "../common/Layout.hpp"
#include "../common/WindowTools.hpp"
#include "settings-widgets/DirectorySelector.hpp"
#include "settings-widgets/FontSelector.h"
#include "WindowSettings.h"

#include <QByteArray>
#include <QDebug>
#include <QDockWidget>
#include <QFont>
#include <QGridLayout>
#include <QRect>
#include <QTextOption>

static constexpr char DATA[] = "Data";
static constexpr char PROJECTS[] = "Projects path";
static const auto PROJECTS_FALLBACK = Path::system(Path::System::Documents) / "Fernanda";

static constexpr char EDITOR[] = "Editor";
static constexpr char COS[] = "Center on scroll";
static constexpr bool COS_FALLBACK = false;
static constexpr char FONT[] = "Font";
static const auto FONT_FALLBACK = QFont("mononoki", 12);
//static constexpr char TAB_STOP[] = "Tab stop distance (px)";
//static constexpr int TAB_STOP_FALLBACK = 40;
static constexpr char TYPEWRITER[] = "Typewriter";
static constexpr bool TYPEWRITER_FALLBACK = false;
static constexpr char WRAP[] = "Wrap";
static const auto WRAP_FALLBACK = static_cast<int>(QTextOption::WrapAtWordBoundaryOrAnywhere);

static constexpr char METER[] = "Meter";
static constexpr char LINE_POS[] = "Line position";
static constexpr bool LINE_POS_FALLBACK = true;
static constexpr char COL_POS[] = "Column position";
static constexpr bool COL_POS_FALLBACK = true;
static constexpr char LINES[] = "Line count";
static constexpr bool LINES_FALLBACK = false;
static constexpr char WORDS[] = "Word count";
static constexpr bool WORDS_FALLBACK = false;
static constexpr char CHARS[] = "Character count";
static constexpr bool CHARS_FALLBACK = false;
static constexpr char POS_LABELS[] = "Position labels";
static constexpr bool POS_LABELS_FALLBACK = true;
static constexpr char COUNT_LABELS[] = "Count labels";
static constexpr bool COUNT_LABELS_FALLBACK = true;
static constexpr char SHORT_LABELS[] = "Use short labels";
static constexpr bool SHORT_LABELS_FALLBACK = true;

static constexpr char WINDOW[] = "Window";
static constexpr char DOCK_POS[] = "Dock position";
static const auto DOCK_POS_FALLBACK = static_cast<int>(Qt::LeftDockWidgetArea);
static constexpr char DOCK_VIS[] = "Dock visibile";
static constexpr bool DOCK_VIS_FALLBACK = true;
static constexpr char GEOMETRY[] = "Geometry";

static constexpr char INI_FILLER[] = "_";
static constexpr char METER_LINE_POS_SHORT[] = "ln";
static constexpr char METER_COL_POS_SHORT[] = "col";
static constexpr char METER_LINES_SHORT[] = "lines";
static constexpr char METER_WORDS_SHORT[] = "words";
static constexpr char METER_CHARS_SHORT[] = "chars";

WindowSettings::WindowSettings(const Path& config, QObject* parent)
	: QObject(parent), m_iniWriter(new IniWriter(config, this))
{
	loadAllIniValues();
}

WindowSettings::~WindowSettings()
{
	qDebug() << __FUNCTION__;

	saveAllIniValues();
}

void WindowSettings::yoke(Window* window)
{
	m_windows << window;

	applyAllTo(window);
	window->installEventFilter(this);

	connect(window->m_dockWidget, &QDockWidget::dockLocationChanged, this, [&](Qt::DockWidgetArea area) {
		passiveApply(WINDOW, DOCK_POS, static_cast<int>(area));
		});
	connect(window->m_dockWidget, &QDockWidget::visibilityChanged, this, [=](bool visible) {
		if (window->isVisible())
			passiveApply<bool>(WINDOW, DOCK_VIS, visible);
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
		passiveApply<QByteArray>(WINDOW, GEOMETRY, window->saveGeometry());

		return true;
	}

	return false;
}

QString WindowSettings::iniName(QString text) const
{
	return text.replace(" ", INI_FILLER).toLower();
}

void WindowSettings::loadAllIniValues()
{
	loadIniValues(DATA, {
		SettingInfo(PROJECTS, PROJECTS_FALLBACK, &WindowSettings::setDataProjectsPath)
		});

	loadIniValues(EDITOR, {
		SettingInfo(COS, COS_FALLBACK, &WindowSettings::setEditorCenterOnScroll),
		SettingInfo(FONT, FONT_FALLBACK, &WindowSettings::setEditorFont),
		SettingInfo(TYPEWRITER, TYPEWRITER_FALLBACK, &WindowSettings::setEditorTypewriter),
		SettingInfo(WRAP, WRAP_FALLBACK, &WindowSettings::setEditorWrap)
		});

	loadIniValues(METER, {
		SettingInfo(LINE_POS, LINE_POS_FALLBACK, &WindowSettings::setMeterLinePosition),
		SettingInfo(COL_POS, COL_POS_FALLBACK, &WindowSettings::setMeterColumnPosition),
		SettingInfo(LINES, LINES_FALLBACK, &WindowSettings::setMeterLineCount),
		SettingInfo(WORDS, WORDS_FALLBACK, &WindowSettings::setMeterWordCount),
		SettingInfo(CHARS, CHARS_FALLBACK, &WindowSettings::setMeterCharCount),
		SettingInfo(POS_LABELS, POS_LABELS_FALLBACK, &WindowSettings::setMeterPositionLabels),
		SettingInfo(COUNT_LABELS, COUNT_LABELS_FALLBACK, &WindowSettings::setMeterCountLabels),
		SettingInfo(SHORT_LABELS, SHORT_LABELS_FALLBACK, &WindowSettings::setMeterShortLabels)
		});

	loadIniValues(WINDOW, {
		SettingInfo(DOCK_POS, DOCK_POS_FALLBACK, &WindowSettings::setWindowDockPosition),
		SettingInfo(DOCK_VIS, DOCK_VIS_FALLBACK, &WindowSettings::setWindowDockVisibility),
		SettingInfo(GEOMETRY, &WindowSettings::setWindowGeometry)
		});
}

void WindowSettings::loadIniValues(const QString& prefix, QList<SettingInfo> infos)
{
	m_iniWriter->begin(iniName(prefix));
	QHash<QString, Setting<Window*>> hash;

	for (auto& info : infos) {
		auto& key = info.key;
		auto variant = m_iniWriter->load(iniName(key), info.fallback);

		hash[key] = Setting<Window*>(variant, this, info.action);
	}

	m_settings[prefix] = hash;
	m_iniWriter->end();
}

void WindowSettings::saveAllIniValues()
{
	saveIniValues(DATA, {
		PROJECTS
		});

	saveIniValues(EDITOR, {
		COS,
		FONT,
		TYPEWRITER
		});

	saveIniValues(METER, {
		LINE_POS,
		COL_POS,
		LINES,
		WORDS,
		CHARS,
		POS_LABELS,
		COUNT_LABELS,
		SHORT_LABELS
		});

	saveIniValues(WINDOW, {
		DOCK_POS,
		DOCK_VIS,
		GEOMETRY
		});
}

void WindowSettings::saveIniValues(const QString& prefix, QStringList keys)
{
	m_iniWriter->begin(iniName(prefix));

	for (auto& key : keys)
		m_iniWriter->save(iniName(key), variantAt(prefix, key));

	m_iniWriter->end();
}

void WindowSettings::applyToAll(const QString& prefix, const QString& key)
{
	for (auto& window : m_windows)
		m_settings[prefix][key].set(window);
}

void WindowSettings::applyAllTo(Window* window)
{
	for (auto it = m_settings.begin(); it != m_settings.end(); ++it)
		for (auto sub_it = it.value().begin(); sub_it != it.value().end(); ++sub_it)
			sub_it.value().set(window);
}

QVariant WindowSettings::variantAt(const QString& prefix, const QString& key) const
{
	return m_settings[prefix][key].variant();
}

void WindowSettings::setDataProjectsPath(Window* window)
{
	auto& setting = m_settings[DATA][PROJECTS];
	auto dir = setting.value<Path>();

	if (!dir.isValid()) {
		setting.setValue<Path>(PROJECTS_FALLBACK);
		dir = PROJECTS_FALLBACK;
	}

	if (dir == PROJECTS_FALLBACK && !dir.isValid())
		Path::createDirs(dir);

	window->m_treeView->setRoot(dir);
}

void WindowSettings::setEditorCenterOnScroll(Window* window)
{
	auto& setting = m_settings[EDITOR][COS];
	auto cos = setting.value<bool>();

	window->m_editorsCos = cos;

	for (auto& editor : window->editors())
		editor->setCenterOnScroll(cos);
}

void WindowSettings::setEditorFont(Window* window)
{
	auto& setting = m_settings[EDITOR][FONT];
	auto font = setting.value<QFont>();
	auto point_size = font.pointSize();

	if (!font.exactMatch() && (point_size < 6 || point_size > 144)) {
		setting.setValue<QFont>(FONT_FALLBACK);
		font = FONT_FALLBACK;
	}

	window->m_editorsFont = font;

	for (auto& editor : window->editors())
		editor->setFont(font);
}

void WindowSettings::setEditorTypewriter(Window* window)
{
	auto& setting = m_settings[EDITOR][TYPEWRITER];
	auto is_typewriter = setting.value<bool>();

	window->m_editorsIsTypewriter = is_typewriter;

	for (auto& editor : window->editors())
		editor->setIsTypewriter(is_typewriter);
}

void WindowSettings::setEditorWrap(Window* window)
{
	auto& setting = m_settings[EDITOR][WRAP];
	auto wrap_mode = static_cast<QTextOption::WrapMode>(setting.value<int>());

	if (wrap_mode == QTextOption::ManualWrap) {
		setting.setValue(static_cast<int>(QTextOption::NoWrap));
		wrap_mode = QTextOption::NoWrap;
	}

	/* UNFINISHED */

	/*Need a way to reset to fallback if int != an enum value?*/

	/*Pool only needed to compare against and populate drop downs.*/

	/*Need an isInPool() function to replace some of these checks, like the above.*/

	window->m_editorsWrapPolicy = wrap_mode;

	for (auto& editor : window->editors())
		editor->setWordWrapMode(wrap_mode);
}

void WindowSettings::setMeterLinePosition(Window* window)
{
	window->m_meter->setHasLinePosition(valueAt<bool>(METER, LINE_POS));
}

void WindowSettings::setMeterColumnPosition(Window* window)
{
	window->m_meter->setHasColumnPosition(valueAt<bool>(METER, COL_POS));
}

void WindowSettings::setMeterLineCount(Window* window)
{
	window->m_meter->setHasLineCount(valueAt<bool>(METER, LINES));
}

void WindowSettings::setMeterWordCount(Window* window)
{
	window->m_meter->setHasWordCount(valueAt<bool>(METER, WORDS));
}

void WindowSettings::setMeterCharCount(Window* window)
{
	window->m_meter->setHasCharCount(valueAt<bool>(METER, CHARS));
}

void WindowSettings::setMeterPositionLabels(Window* window)
{
	auto& setting = m_settings[METER][POS_LABELS];
	window->m_meter->setHasPositionLabels(setting.value<bool>());

	window->m_meter->run();
}

void WindowSettings::setMeterCountLabels(Window* window)
{
	auto& setting = m_settings[METER][COUNT_LABELS];
	window->m_meter->setHasCountLabels(setting.value<bool>());

	window->m_meter->run();
}

void WindowSettings::setMeterShortLabels(Window* window)
{
	auto& setting = m_settings[METER][SHORT_LABELS];
	auto value = setting.value<bool>();

	auto set_text = [&](const QString& shortLabel) { return value ? shortLabel : QString(); };

	window->m_meter->setLinePositionLabel(set_text(METER_LINE_POS_SHORT));
	window->m_meter->setColumnPositionLabel(set_text(METER_COL_POS_SHORT));
	window->m_meter->setLineCountLabel(set_text(METER_LINES_SHORT));
	window->m_meter->setWordCountLabel(set_text(METER_WORDS_SHORT));
	window->m_meter->setCharCountLabel(set_text(METER_CHARS_SHORT));

	window->m_meter->run();
}

void WindowSettings::setWindowDockPosition(Window* window)
{
	auto& setting = m_settings[WINDOW][DOCK_POS];
	auto area = static_cast<Qt::DockWidgetArea>(setting.value<int>());

	if (area == Qt::NoDockWidgetArea) {
		setting.setValue<int>(DOCK_POS_FALLBACK);
		area = static_cast<Qt::DockWidgetArea>(DOCK_POS_FALLBACK);
	}

	window->addDockWidget(area, window->m_dockWidget);
}

void WindowSettings::setWindowDockVisibility(Window* window)
{
	window->m_dockWidget->setVisible(valueAt<bool>(WINDOW, DOCK_VIS));
}

void WindowSettings::setWindowGeometry(Window* window)
{
	auto& setting = m_settings[WINDOW][GEOMETRY];
	auto byte_array = setting.value<QByteArray>();

	if (!window->restoreGeometry(byte_array))
		window->setGeometry(QRect(0, 0, 1000, 600));

	WindowTools::moveXYIfTaken(window, m_windows);
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
		Layout::box(Layout::Orientation::Horizontal, group_box, QWidgetList{ combo_box });
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
	auto initial_dir = valueAt<Path>(DATA, PROJECTS);
	auto dir_selector = new DirectorySelector(dialog, initial_dir);

	connect(dir_selector, &DirectorySelector::selected, this, [&](const Path& directory) {
		activeApply<Path>(DATA, PROJECTS, directory);
		});

	auto layout = Layout::box(Layout::Orientation::Horizontal, QWidgetList{ dir_selector });

	auto box = newGroupBox(dialog, layout, margins, spacing);
	box->setTitle(PROJECTS);

	return box;
}

QGroupBox* WindowSettings::newEditorBox(QDialog* dialog, const QMargins& margins, int spacing)
{
	QWidgetList check_boxes = {
		newCheckBox(EDITOR, COS, dialog),
		newCheckBox(EDITOR, TYPEWRITER, dialog)
	};

	auto bottom_layout = Layout::box(Layout::Orientation::Horizontal, check_boxes);

	QObjectList objects = {
		newFontBox(dialog, margins, spacing),
		bottom_layout
	};

	auto layout = Layout::box(Layout::Orientation::Vertical, objects);

	auto box = newGroupBox(dialog, layout, margins, spacing);
	box->setTitle(EDITOR);

	return box;
}

QGroupBox* WindowSettings::newFontBox(QDialog* dialog, const QMargins& margins, int spacing)
{
	auto initial_font = valueAt<QFont>(EDITOR, FONT);
	auto font_selector = new FontSelector(dialog, initial_font);

	connect(font_selector, &FontSelector::currentFontChanged, this, [&](const QFont& font) {
		activeApply<QFont>(EDITOR, FONT, font);
		});

	auto layout = Layout::box(Layout::Orientation::Horizontal, QWidgetList{ font_selector });
	// ^ Overload to take 1 widget/object

	auto box = newGroupBox(dialog, layout, margins, spacing);
	box->setTitle(FONT);

	return box;
}

QGroupBox* WindowSettings::newMeterBox(QDialog* dialog, const QMargins& margins, int spacing)
{
	// Toggle/untoggle all button.

	QWidgetList check_boxes = {
		newCheckBox(METER, LINE_POS, dialog),
		newCheckBox(METER, COL_POS, dialog),
		newCheckBox(METER, LINES, dialog),
		newCheckBox(METER, WORDS, dialog),
		newCheckBox(METER, CHARS, dialog),
		newCheckBox(METER, POS_LABELS, dialog),
		newCheckBox(METER, COUNT_LABELS, dialog),
		newCheckBox(METER, SHORT_LABELS, dialog)
	};

	auto grid = new QGridLayout;
	Layout::evenlyAddToGrid(grid, check_boxes, 5);

	auto box = newGroupBox(dialog, grid, margins, spacing);
	box->setTitle(METER);

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

void WindowSettings::onDialogFinished()
{
	delete m_dialog;

	m_dialog = nullptr;
}
