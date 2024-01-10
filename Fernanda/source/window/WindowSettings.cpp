#include "../common/Connect.hpp"
#include "../menu/CheckBoxGroup.hpp"
#include "WindowSettings.h"

#include <QGridLayout>
#include <QMargins>

constexpr char METER_LINE_POS[] = "Line position";
constexpr char METER_COL_POS[] = "Column position";
constexpr char METER_LINES[] = "Line count";
constexpr char METER_WORDS[] = "Word count";
constexpr char METER_CHARS[] = "Character count";

WindowSettings::WindowSettings(const Path& config, QObject* parent)
	: QObject(parent), m_iniWriter(new IniWriter(config, this))
{
	setupMeterGroup();
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

void WindowSettings::applyAll(Window* window)
{
	for (auto& group : actionGroups())
		for (auto& action : group->actions())
			applySettingByAction(window, action);
}

void WindowSettings::applyAll(QList<Window*>& windows)
{
	for (auto& window : windows)
		applyAll(window);
}

void WindowSettings::applySetting(Window* window, Type type)
{
	applyCheckedSettingByType(window, type);
}

void WindowSettings::applySetting(QList<Window*>& windows, Type type)
{
	for (auto& window : windows)
		applySetting(window, type);
}

void WindowSettings::applySettingByAction(Window* window, QAction* action)
{
	auto type = typeOf(action);

	applyCheckedSettingByType(window, type);
}

void WindowSettings::applyCheckedSettingByType(Window* window, Type type)
{
	auto action = actionWith(type);
	auto checked = action->isChecked();

	switch (type) {
	case WindowSettings::Type::MeterLinePos:
		window->m_meter->setHasLinePosition(checked);
		break;
	case WindowSettings::Type::MeterColPos:
		window->m_meter->setHasColumnPosition(checked);
		break;
	case WindowSettings::Type::MeterLineCount:
		window->m_meter->setHasLineCount(checked);
		break;
	case WindowSettings::Type::MeterWordCount:
		window->m_meter->setHasWordCount(checked);
		break;
	case WindowSettings::Type::MeterCharCount:
		window->m_meter->setHasCharCount(checked);
		break;
	}
}

WindowSettings::Type WindowSettings::typeOf(QAction* action)
{
	return dataOf(action).type;
}

WindowSettings::ActionData WindowSettings::dataOf(QAction* action)
{
	return action->data().value<ActionData>();
}

QAction* WindowSettings::actionWith(Type type)
{
	for (auto& group : actionGroups())
		for (auto& action : group->actions())
			if (typeOf(action) == type)
				return action;

	return nullptr;
}

void WindowSettings::setupDialog(QDialog* dialog)
{
	auto grid = new QGridLayout(dialog);
	grid->setContentsMargins(QMargins(5, 5, 5, 5));
	grid->setSpacing(5);

	dialog->setLayout(grid);
	dialog->setFixedSize(800, 500);

	auto meter_box = new CheckBoxGroup(m_meterGroup, CheckBoxGroup::Align::Horizontal, dialog);
	grid->addWidget(meter_box);

	connect(m_dialog, &QDialog::finished, this, &WindowSettings::onQDialogFinished);
}

void WindowSettings::setupMeterGroup()
{
	newAction(m_meterGroup, METER_LINE_POS, Type::MeterLinePos, true);
	newAction(m_meterGroup, METER_COL_POS, Type::MeterColPos, true);
	newAction(m_meterGroup, METER_LINES, Type::MeterLineCount, false);
	newAction(m_meterGroup, METER_WORDS, Type::MeterWordCount, false);
	newAction(m_meterGroup, METER_CHARS, Type::MeterCharCount, false);

	m_meterGroup->setAllCheckable(true);
	m_meterGroup->setExclusive(false);

	loadGroupValues(m_meterGroup);
}

QList<ActionGroup*> WindowSettings::actionGroups() const
{
	return findChildren<ActionGroup*>();
}

WindowSettings::ActionData WindowSettings::dataOf(QAction* action) const
{
	auto data = action->data();

	return data.value<ActionData>();
}

QString WindowSettings::iniKeyName(QAction* action) const
{
	return action->text().replace(" ", "_");
}

void WindowSettings::loadGroupValues(ActionGroup* group)
{
	m_iniWriter->begin(group->name());

	for (auto& action : group->actions()) {
		auto key = iniKeyName(action);
		auto action_data = dataOf(action);

		auto& default_value = action_data.fallback;
		auto state = m_iniWriter->load<bool>(key, default_value);
		
		action->setChecked(state);
	}

	m_iniWriter->end();
}

void WindowSettings::onQActionToggled(bool checked)
{
	auto action = sender_cast(QAction);
	if (!action) return;

	qDebug() << __FUNCTION__;

	// Notify Fernanda and then have Fernanda send list of current Windows to have the setting applied?
	// Where to save value? No need to write all until close, is there?
	// The current needed value is stored in each group's actions.

	auto type = typeOf(action);

	emit settingChanged(type);
}

void WindowSettings::onQDialogFinished()
{
	delete m_dialog;
	m_dialog = nullptr;
}
