#include "../common/Connect.hpp"
#include "../menu/ActionsUiBoxes.hpp"
#include "WindowSettings.h"

#include <QGridLayout>
#include <QMargins>

constexpr char METER_LINE_POS[] = "Line position";
constexpr char METER_COL_POS[] = "Column position";
constexpr char METER_LINES[] = "Line count";
constexpr char METER_WORDS[] = "Word count";
constexpr char METER_CHARS[] = "Character count";
constexpr char INI_FILLER[] = "_";

WindowSettings::WindowSettings(const Path& config, QObject* parent)
	: QObject(parent), m_iniWriter(new IniWriter(config, this))
{
	setupMeterActionsMap();
}

WindowSettings::~WindowSettings()
{
	qDebug() << __FUNCTION__;

	for (auto& actions_map : actionsMaps())
		saveActionsMapValues(actions_map);

	/*for (auto& groups_map : groupsMaps())
		saveGroupsMapValues(groups_map);*/
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
	auto value = currentValue(type);
	if (!value.isValid()) return;

	// Will this work for font?

	switch (type) {
	case WindowSettings::Type::MeterLinePos:
		window->m_meter->setHasLinePosition(value.toBool());
		break;
	case WindowSettings::Type::MeterColPos:
		window->m_meter->setHasColumnPosition(value.toBool());
		break;
	case WindowSettings::Type::MeterLineCount:
		window->m_meter->setHasLineCount(value.toBool());
		break;
	case WindowSettings::Type::MeterWordCount:
		window->m_meter->setHasWordCount(value.toBool());
		break;
	case WindowSettings::Type::MeterCharCount:
		window->m_meter->setHasCharCount(value.toBool());
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
	return findChildren<ActionsMap*>();
}

QList<ActionGroupsMap*> WindowSettings::groupsMaps() const
{
	return findChildren<ActionGroupsMap*>();
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
		auto name = actionsMap->actionName(action);
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
		auto name = actionsMap->actionName(action);
		auto key = iniKeyName(name);

		auto state = actionsMap->state(action);
		m_iniWriter->save(key, state);
	}

	m_iniWriter->end();
}

/*void WindowSettings::loadGroupsMapValues(ActionGroupsMap* groupsMap)
{
	// Test with new ActionGroupDropDown

	m_iniWriter->begin(groupsMap->name());

	for (auto& group : groupsMap->groups()) {
		auto name = groupsMap->groupName(group);
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
		auto name = groupsMap->groupName(group);
		auto key = iniKeyName(name);

		auto state = groupsMap->state(group);
		m_iniWriter->save(key, state);
	}

	m_iniWriter->end();
}*/

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
			return actions_map->actionData(action).value<Type>();

	return Type();
}

QVariant WindowSettings::currentValue(Type type)
{
	// The shared base and pure virtual `state` probably means these loops can be combined

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
