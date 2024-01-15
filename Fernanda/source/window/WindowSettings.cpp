#include "../common/Connect.hpp"
#include "settings-widgets/ActionsUiBoxes.h"
#include "settings-widgets/FontSelector.h"
#include "WindowSettings.h"

#include <QGridLayout>
#include <QMargins>

// For testing
#include <QFontDialog>

constexpr char FONT[] = "Font";
constexpr char INI_FILLER[] = "_";
constexpr char METER_LINE_POS[] = "Line position";
constexpr char METER_COL_POS[] = "Column position";
constexpr char METER_LINES[] = "Line count";
constexpr char METER_WORDS[] = "Word count";
constexpr char METER_CHARS[] = "Character count";

WindowSettings::WindowSettings(const Path& config, QObject* parent)
	: QObject(parent), m_iniWriter(new IniWriter(config, this))
{
	loadFont();
	setupMeterActionsMap();
}

WindowSettings::~WindowSettings()
{
	qDebug() << __FUNCTION__;

	saveFont();
	saveAllMapsValues();
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

void WindowSettings::applySetting(Window* window, Type type)
{
	auto variant = currentValue(type);
	if (!variant.isValid()) return;

	switch (type) {
	case Type::Font:
		applyFont(window, variant.value<QFont>());
		break;
	case Type::MeterLinePos:
		window->m_meter->setHasLinePosition(variant.toBool());
		break;
	case Type::MeterColPos:
		window->m_meter->setHasColumnPosition(variant.toBool());
		break;
	case Type::MeterLineCount:
		window->m_meter->setHasLineCount(variant.toBool());
		break;
	case Type::MeterWordCount:
		window->m_meter->setHasWordCount(variant.toBool());
		break;
	case Type::MeterCharCount:
		window->m_meter->setHasCharCount(variant.toBool());
		break;
	}
}

void WindowSettings::applySetting(QList<Window*>& windows, Type type)
{
	for (auto& window : windows)
		applySetting(window, type);
}

void WindowSettings::applyAll(Window* window)
{
	for (auto i = 0; i < static_cast<int>(Type::End); ++i)
		applySetting(window, static_cast<Type>(i));
}

void WindowSettings::applyAll(QList<Window*>& windows)
{
	for (auto& window : windows)
		applyAll(window);
}

QList<ActionsMap*> WindowSettings::actionsMaps() const
{
	return findChildren<ActionsMap*>(Qt::FindDirectChildrenOnly);
}

QList<ActionGroupsMap*> WindowSettings::groupsMaps() const
{
	return findChildren<ActionGroupsMap*>(Qt::FindDirectChildrenOnly);
}

void WindowSettings::setupMeterActionsMap()
{
	m_meterActionsMap->add(METER_LINE_POS, toVariant(Type::MeterLinePos), true);
	m_meterActionsMap->add(METER_COL_POS, toVariant(Type::MeterColPos), true);
	m_meterActionsMap->add(METER_LINES, toVariant(Type::MeterLineCount), false);
	m_meterActionsMap->add(METER_WORDS, toVariant(Type::MeterWordCount), false);
	m_meterActionsMap->add(METER_CHARS, toVariant(Type::MeterCharCount), false);

	m_meterActionsMap->setAllCheckable(true);
	loadActionsMapValues(m_meterActionsMap);

	for (auto& action : m_meterActionsMap->actions())
		connect(action, &QAction::toggled, this, &WindowSettings::onQActionToggled);
}

void WindowSettings::loadActionsMapValues(ActionsMap* actionsMap)
{
	m_iniWriter->begin(actionsMap->name());

	for (auto& action : actionsMap->actions()) {
		auto name = actionsMap->nameOf(action);
		auto key = iniKeyName(name);
		auto fallback = actionsMap->fallback(action);

		auto state = m_iniWriter->load(key, fallback);
		actionsMap->setState(action, state);
	}

	m_iniWriter->end();
}

void WindowSettings::saveActionsMapValues(ActionsMap* actionsMap)
{
	m_iniWriter->begin(actionsMap->name());

	for (auto& action : actionsMap->actions()) {
		auto name = actionsMap->nameOf(action);
		auto key = iniKeyName(name);

		auto state = actionsMap->state(action);
		m_iniWriter->save(key, state);
	}

	m_iniWriter->end();
}

void WindowSettings::saveAllMapsValues()
{
	for (auto& actions_map : actionsMaps())
		saveActionsMapValues(actions_map);

	/*
	for (auto& groups_map : groupsMaps())
		saveGroupsMapValues(groups_map);
		*/
}

/*
void WindowSettings::loadGroupsMapValues(ActionGroupsMap* groupsMap)
{
	// Test with new ActionGroupDropDown

	m_iniWriter->begin(groupsMap->name());

	for (auto& group : groupsMap->groups()) {
		auto name = groupsMap->nameOf(group);
		auto key = iniKeyName(name);
		auto fallback = groupsMap->fallback(group);

		auto state = m_iniWriter->load(key, fallback);
		groupsMap->setState(group, state);
	}

	m_iniWriter->end();
}

void WindowSettings::saveGroupsMapValues(ActionGroupsMap* groupsMap)
{
	m_iniWriter->begin(groupsMap->name());

	for (auto& group : groupsMap->groups()) {
		auto name = groupsMap->nameOf(group);
		auto key = iniKeyName(name);

		auto state = groupsMap->state(group);
		m_iniWriter->save(key, state);
	}

	m_iniWriter->end();
}
*/

void WindowSettings::loadFont()
{
	m_iniWriter->begin(FONT);
	auto font = m_iniWriter->load(FONT, QFont("Mononoki", 12));
	m_currentFont = font.value<QFont>();

	m_iniWriter->end();
}

void WindowSettings::saveFont()
{
	m_iniWriter->begin(FONT);
	m_iniWriter->save(FONT, m_currentFont);

	m_iniWriter->end();
}

void WindowSettings::applyFont(Window* window, QFont font)
{
	window->m_editorFont = font;

	for (auto& editor : window->editors())
		editor->setFont(font);
}

QString WindowSettings::iniKeyName(QString text)
{
	return text.replace(" ", INI_FILLER);
}

QVariant WindowSettings::toVariant(Type type)
{
	return QVariant::fromValue<Type>(type);
}

void WindowSettings::setupDialog(QDialog* dialog)
{
	auto grid = new QGridLayout(dialog);
	grid->setContentsMargins(QMargins(5, 5, 5, 5));
	grid->setSpacing(5);

	dialog->setLayout(grid);
	dialog->setFixedSize(800, 500);

	// TESTING

	auto font_box = new QGroupBox(dialog);
	auto font_selector = new FontSelector(m_currentFont, font_box);
	font_box->setTitle(FONT);
	Layout::box(Box::Horizontal, font_box, QWidgetList{ font_selector });
	// ^-- Overload to take 1 widget/object
	grid->addWidget(font_box);
	connect(font_selector, &FontSelector::currentFontChanged, this, &WindowSettings::onFontSelectorFontChanged);

	// For testing
	//auto debug_font_dialog = new QFontDialog(dialog);
	//debug_font_dialog->show();
	//connect(debug_font_dialog, &QFontDialog::currentFontChanged, this, &WindowSettings::onFontSelectorFontChanged);

	// TESTING

	auto meter_actions = m_meterActionsMap->actions();
	auto meter_box = new ActionsChecksBox(meter_actions, ActionsChecksBox::Align::Horizontal, dialog);
	meter_box->setTitle(m_meterActionsMap->name());
	grid->addWidget(meter_box);

	connect(m_dialog, &QDialog::finished, this, &WindowSettings::onQDialogFinished);
}

WindowSettings::Type WindowSettings::typeData(QAction* action)
{
	// Overload for groups type data?

	for (auto& actions_map : actionsMaps())
		if (actions_map->contains(action))
			return actions_map->dataOf(action).value<Type>();

	return Type();
}

QVariant WindowSettings::currentValue(Type type)
{
	// This sucks.

	if (type == Type::Font)
		return m_currentFont;

	auto type_variant = toVariant(type);

	for (auto& actions_map : actionsMaps()) {
		auto action = actions_map->keyWith(type_variant);

		if (action)
			return actions_map->state(action);
	}

	for (auto& groups_map : groupsMaps()) {
		auto group = groups_map->keyWith(type_variant);

		if (group)
			return groups_map->state(group);
	}

	return QVariant();
}

void WindowSettings::onQActionToggled(bool)
{
	auto action = sender_cast(QAction);
	if (!action) return;

	// Notify Fernanda and then have Fernanda send list of current Windows to have the setting applied

	auto type = typeData(action);

	emit settingChanged(type);
}

void WindowSettings::onQDialogFinished()
{
	delete m_dialog;

	m_dialog = nullptr;
}

void WindowSettings::onFontSelectorFontChanged(const QFont& font)
{
	qDebug() << __FUNCTION__ << m_currentFont << ", to: " << font;

	m_currentFont = font;

	emit settingChanged(Type::Font);
}
