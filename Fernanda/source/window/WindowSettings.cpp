#include "../common/Connect.hpp"
#include "../menu/ActionCheckBoxes.hpp"
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
	setupMeterActionSet();
}

WindowSettings::~WindowSettings()
{
	qDebug() << __FUNCTION__;

	for (auto& action_set : actionSets())
		saveActionSetValues(action_set);

	for (auto& group_set : groupSets())
		saveGroupSetValues(group_set);
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

QList<ActionSet*> WindowSettings::actionSets() const
{
	return findChildren<ActionSet*>();
}

QList<ActionGroupSet*> WindowSettings::groupSets() const
{
	return findChildren<ActionGroupSet*>();
}

void WindowSettings::setupMeterActionSet()
{
	m_meterActionSet->add(METER_LINE_POS, toVariant(Type::MeterLinePos), true);
	m_meterActionSet->add(METER_COL_POS, toVariant(Type::MeterColPos), true);
	m_meterActionSet->add(METER_LINES, toVariant(Type::MeterLineCount), false);
	m_meterActionSet->add(METER_WORDS, toVariant(Type::MeterWordCount), false);
	m_meterActionSet->add(METER_CHARS, toVariant(Type::MeterCharCount), false);

	m_meterActionSet->setAllCheckable(true);
	loadActionSetValues(m_meterActionSet);

	for (auto& action : m_meterActionSet->actions())
		connect(action, &QAction::toggled,
			this, &WindowSettings::onQActionToggled);
}

void WindowSettings::loadActionSetValues(ActionSet* actionSet)
{
	m_iniWriter->begin(actionSet->name());

	for (auto& action : actionSet->actions()) {
		auto name = actionSet->actionName(action);
		auto key = iniKeyName(name);
		auto fallback = actionSet->fallback(action);
		auto state = m_iniWriter->load<bool>(key, fallback);

		action->setChecked(state);
	}

	m_iniWriter->end();
}

void WindowSettings::saveActionSetValues(ActionSet* actionSet)
{
	m_iniWriter->begin(actionSet->name());

	for (auto& action : actionSet->actions()) {
		auto name = actionSet->actionName(action);
		auto key = iniKeyName(name);

		m_iniWriter->save(key, action->isChecked());
	}

	m_iniWriter->end();
}

void WindowSettings::loadGroupSetValues(ActionGroupSet* groupSet)
{
	//
}

void WindowSettings::saveGroupSetValues(ActionGroupSet* groupSet)
{
	//
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

	auto meter_actions = m_meterActionSet->actions();
	auto meter_box = new CheckBoxGroup(meter_actions, CheckBoxGroup::Align::Horizontal, dialog);
	grid->addWidget(meter_box);

	connect(m_dialog, &QDialog::finished, this, &WindowSettings::onQDialogFinished);
}

WindowSettings::Type WindowSettings::typeData(QAction* action)
{
	for (auto& action_set : actionSets())
		if (action_set->contains(action))
			return action_set->actionData(action).value<Type>();

	return Type();
}

QVariant WindowSettings::currentValue(Type type)
{
	auto type_variant = toVariant(type);

	for (auto& action_set : actionSets()) {
		auto action = action_set->itemWith(type_variant);

		if (action)
			return action->isChecked();
	}

	for (auto& group_set : groupSets()) {
		auto group = group_set->itemWith(type_variant);

		if (group)
			return group->checkedAction()->data();
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
