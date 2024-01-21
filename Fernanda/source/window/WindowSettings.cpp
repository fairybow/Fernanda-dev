#include "../common/Layout.hpp"
#include "settings-widgets/FontSelector.h"
#include "WindowSettings.h"

#include <QGridLayout>
#include <QGroupBox>

constexpr char INI_FILLER[] = "_";
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
}

void WindowSettings::detach(Window* window)
{
	m_windows.removeAll(window);
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

void WindowSettings::loadAll()
{
	loadEditorSettings();
	loadMeterSettings();
}

void WindowSettings::loadEditorSettings()
{
	m_iniWriter->begin(EDITOR);
	QMap<QString, Action> editor_settings;

	editor_settings[EDITOR_FONT] = {
		m_iniWriter->load(iniName(EDITOR_FONT), QFont(EDITOR_FONT_DEFAULT, EDITOR_FONTSIZE_DEFAULT)),

		[action = &editor_settings[EDITOR_FONT]](Window* window) {
			window->m_editorFont = action->variant.value<QFont>();

			for (auto& editor : window->editors())
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
	QMap<QString, Action> meter_settings;

	meter_settings[METER_LINE_POS] = {
		m_iniWriter->load(iniName(METER_LINE_POS), true),

		[action = &meter_settings[METER_LINE_POS]](Window* window) {
			window->m_meter->setHasLinePosition(action->variant.toBool());
		}
	};

	meter_settings[METER_COL_POS] = {
		m_iniWriter->load(iniName(METER_COL_POS), true),

		[action = &meter_settings[METER_COL_POS]](Window* window) {
			window->m_meter->setHasColumnPosition(action->variant.toBool());
		}
	};

	meter_settings[METER_LINES] = {
		m_iniWriter->load(iniName(METER_LINES), false),

		[action = &meter_settings[METER_LINES]](Window* window) {
			window->m_meter->setHasLineCount(action->variant.toBool());
		}
	};

	meter_settings[METER_WORDS] = {
		m_iniWriter->load(iniName(METER_WORDS), false),

		[action = &meter_settings[METER_WORDS]](Window* window) {
			window->m_meter->setHasWordCount(action->variant.toBool());
		}
	};

	meter_settings[METER_CHARS] = {
		m_iniWriter->load(iniName(METER_CHARS), false),

		[action = &meter_settings[METER_CHARS]](Window* window) {
			window->m_meter->setHasCharCount(action->variant.toBool());
		}
	};

	m_settings[METER] = meter_settings;
	m_iniWriter->end();
}

void WindowSettings::saveAll()
{
	saveEditorSettings();
	saveMeterSettings();
}

void WindowSettings::saveEditorSettings()
{
	m_iniWriter->begin(EDITOR);

	m_iniWriter->save(iniName(EDITOR_FONT), m_settings[EDITOR][EDITOR_FONT].variant);
	//...

	m_iniWriter->end();
}

void WindowSettings::saveMeterSettings()
{
	m_iniWriter->begin(METER);

	m_iniWriter->save(iniName(METER_LINE_POS), m_settings[METER][METER_LINE_POS].variant);
	m_iniWriter->save(iniName(METER_COL_POS), m_settings[METER][METER_COL_POS].variant);
	m_iniWriter->save(iniName(METER_LINES), m_settings[METER][METER_LINES].variant);
	m_iniWriter->save(iniName(METER_WORDS), m_settings[METER][METER_WORDS].variant);
	m_iniWriter->save(iniName(METER_CHARS), m_settings[METER][METER_CHARS].variant);

	m_iniWriter->end();
}

void WindowSettings::applyAll(Window* window)
{
	for (auto it = m_settings.begin(); it != m_settings.end(); ++it)
		for (auto sub_it = it.value().begin(); sub_it != it.value().end(); ++sub_it)
			sub_it.value().action(window);
}

QString WindowSettings::iniName(QString text)
{
	return text.replace(" ", INI_FILLER);
}

void WindowSettings::setupDialog(QDialog* dialog)
{
	auto grid = new QGridLayout(dialog);
	auto space = 5;
	grid->setContentsMargins(space, space, space, space);
	grid->setSpacing(space);

	dialog->setLayout(grid);
	dialog->setFixedSize(800, 500);

	auto font_box = new QGroupBox(dialog);
	auto font_selector = new FontSelector(m_settings[EDITOR][EDITOR_FONT].variant.value<QFont>(), font_box);
	font_box->setTitle(EDITOR_FONT);
	Layout::box(Box::Horizontal, font_box, QWidgetList{ font_selector }); // Overload to take 1 widget/object
	grid->addWidget(font_box);
	connect(font_selector, &FontSelector::currentFontChanged, this, [&](const QFont& font) {
		applySetting<QFont>(EDITOR, EDITOR_FONT, font);
		});

	auto meter_box = new QGroupBox(dialog);
	meter_box->setTitle(METER);
	auto action1 = new QCheckBox(METER_LINE_POS, meter_box);
	auto action2 = new QCheckBox(METER_COL_POS, meter_box);
	auto action3 = new QCheckBox(METER_LINES, meter_box);
	auto action4 = new QCheckBox(METER_WORDS, meter_box);
	auto action5 = new QCheckBox(METER_CHARS, meter_box);

	action1->setChecked(m_settings[METER][METER_LINE_POS].variant.value<bool>());
	action2->setChecked(m_settings[METER][METER_COL_POS].variant.value<bool>());
	action3->setChecked(m_settings[METER][METER_LINES].variant.value<bool>());
	action4->setChecked(m_settings[METER][METER_WORDS].variant.value<bool>());
	action5->setChecked(m_settings[METER][METER_CHARS].variant.value<bool>());

	connect(action1, &QCheckBox::stateChanged, this, [=](int state) {
		applySetting<bool>(METER, METER_LINE_POS, state);
		});
	connect(action2, &QCheckBox::stateChanged, this, [=](int state) {
		applySetting<bool>(METER, METER_COL_POS, state);
		});
	connect(action3, &QCheckBox::stateChanged, this, [=](int state) {
		applySetting<bool>(METER, METER_LINES, state);
		});
	connect(action4, &QCheckBox::stateChanged, this, [=](int state) {
		applySetting<bool>(METER, METER_WORDS, state);
		});
	connect(action5, &QCheckBox::stateChanged, this, [=](int state) {
		applySetting<bool>(METER, METER_CHARS, state);
		});

	auto layout = Layout::box(Box::Horizontal, meter_box, QWidgetList{ action1, action2, action3, action4, action5 });
	layout->setContentsMargins(5, 5, 5, 5);

	grid->addWidget(meter_box);

	connect(m_dialog, &QDialog::finished, this, &WindowSettings::onDialogFinished);

	// v--- Filler

	for (auto i = 0; i < 3; ++i) {
		auto groupBox = new QGroupBox(dialog);
		auto comboBox = new QComboBox(groupBox);
		groupBox->setTitle(QString("Group Box %1").arg(i + 1));
		Layout::box(Box::Horizontal, groupBox, QWidgetList{ comboBox });
		grid->addWidget(groupBox);
	}
}

void WindowSettings::onDialogFinished()
{
	delete m_dialog;

	m_dialog = nullptr;
}
