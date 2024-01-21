#include "../common/Layout.hpp"
#include "settings-widgets/FontSelector.h"
#include "WindowSettings.h"

#include <QByteArray>
#include <QGridLayout>
#include <QGroupBox>
#include <QRect>

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
	loadEditorSettings();
	loadMeterSettings();
	loadWindowSettings();
}

void WindowSettings::loadEditorSettings()
{
	m_iniWriter->begin(EDITOR);
	QMap<QString, Setting> editor_settings;

	editor_settings[EDITOR_FONT] = {
		m_iniWriter->load(iniName(EDITOR_FONT), QFont(EDITOR_FONT_DEFAULT, EDITOR_FONTSIZE_DEFAULT)),

		[setting = &editor_settings[EDITOR_FONT]](Window* window) {
			window->m_editorFont = setting->value<QFont>();

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

		[setting = &window_settings[WINDOW_GEOMETRY]](Window* window) {

			auto byte_array = setting->value<QByteArray>();

			if (byte_array.isNull())
				window->setGeometry(QRect(0, 0, 1000, 600));
			else
				window->restoreGeometry(byte_array);
		}
	};

	m_settings[WINDOW] = window_settings;
	m_iniWriter->end();
}

void WindowSettings::saveAll()
{
	saveEditorSettings();
	saveMeterSettings();
	saveWindowSettings();
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
	auto grid = new QGridLayout(dialog);
	auto space = 5;
	grid->setContentsMargins(space, space, space, space);
	grid->setSpacing(space);

	dialog->setLayout(grid);
	dialog->setFixedSize(800, 500);

	auto font_box = new QGroupBox(dialog);
	auto font_selector = new FontSelector(valueAt<QFont>(EDITOR, EDITOR_FONT), font_box);
	font_box->setTitle(EDITOR_FONT);
	Layout::box(Box::Horizontal, font_box, QWidgetList{ font_selector }); // Overload to take 1 widget/object
	grid->addWidget(font_box);
	connect(font_selector, &FontSelector::currentFontChanged, this, [&](const QFont& font) {
		activeApply<QFont>(EDITOR, EDITOR_FONT, font);
		});

	auto meter_box = new QGroupBox(dialog);
	meter_box->setTitle(METER);
	auto action1 = new QCheckBox(METER_LINE_POS, meter_box);
	auto action2 = new QCheckBox(METER_COL_POS, meter_box);
	auto action3 = new QCheckBox(METER_LINES, meter_box);
	auto action4 = new QCheckBox(METER_WORDS, meter_box);
	auto action5 = new QCheckBox(METER_CHARS, meter_box);

	action1->setChecked(valueAt<bool>(METER, METER_LINE_POS));
	action2->setChecked(valueAt<bool>(METER, METER_COL_POS));
	action3->setChecked(valueAt<bool>(METER, METER_LINES));
	action4->setChecked(valueAt<bool>(METER, METER_WORDS));
	action5->setChecked(valueAt<bool>(METER, METER_CHARS));

	connect(action1, &QCheckBox::stateChanged, this, [&](int state) {
		activeApply<bool>(METER, METER_LINE_POS, state);
		});
	connect(action2, &QCheckBox::stateChanged, this, [&](int state) {
		activeApply<bool>(METER, METER_COL_POS, state);
		});
	connect(action3, &QCheckBox::stateChanged, this, [&](int state) {
		activeApply<bool>(METER, METER_LINES, state);
		});
	connect(action4, &QCheckBox::stateChanged, this, [&](int state) {
		activeApply<bool>(METER, METER_WORDS, state);
		});
	connect(action5, &QCheckBox::stateChanged, this, [&](int state) {
		activeApply<bool>(METER, METER_CHARS, state);
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
