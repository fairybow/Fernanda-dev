#include "../common/Connect.hpp"
#include "../common/Layout.hpp"
#include "../menu/FontBox.hpp"
#include "WindowSettings.h"

#include <QFont>
#include <QGridLayout>
#include <QMargins>
#include <QVariant>

WindowSettings::WindowSettings(const Path& config, QObject* parent)
	: QObject(parent), m_iniWriter(new IniWriter(config, this))
{
	setup();
}

void WindowSettings::openDialog()
{
	if (m_dialog) {
		m_dialog->raise();
		m_dialog->activateWindow();

		return;
	}

	m_dialog = new QDialog;
	setupDialog();

	m_dialog->open();
}

void WindowSettings::applySetting(Window* window, bool checked, Type type)
{
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

	auto prefix_name = iniName(type, Prefix::Yes);
	m_iniWriter->begin(prefix_name);

	m_iniWriter->save(iniName(type), checked);
	m_iniWriter->end();
}

void WindowSettings::applySetting(QList<Window*> windows, bool checked, Type type)
{
	for (auto& window : windows)
		applySetting(window, checked, type);
}

void WindowSettings::setup()
{
	setupMeterActions();
	// Etc.
}

void WindowSettings::setupMeterActions()
{
	QList<QAction*> meter_actions = {
		makeAction(Type::MeterLinePos),
		makeAction(Type::MeterColPos),
		makeAction(Type::MeterLineCount),
		makeAction(Type::MeterWordCount),
		makeAction(Type::MeterCharCount)
	};

	// Retrieve settings here

	for (auto& action : meter_actions) {
		action->setCheckable(true);
		//action->setChecked(); // Set value from settings
		connect(action, &QAction::toggled, this, &WindowSettings::onQActionToggled);
	}
}

QAction* WindowSettings::makeAction(Type type)
{
	auto action = new QAction(this);
	m_namesToActions[name(type)] = action;
	action->setData(QVariant::fromValue(type));

	return action;
}

void WindowSettings::setupDialog()
{
	auto grid = new QGridLayout(m_dialog);
	grid->setContentsMargins(QMargins(5, 5, 5, 5));
	grid->setSpacing(5);

	m_dialog->setLayout(grid);
	m_dialog->setFixedSize(800, 500);

	grid->addWidget(new FontBox(QFont(), "Fonts", m_dialog), 0, 0, 4, 1);
	grid->addWidget(meterBox(), 0, 2, 4, 4);
	grid->addWidget(meterBox(), 0, 6, 4, 4);
	grid->addWidget(meterBox(), 0, 10, 4, 4);

	connect(m_dialog, &QDialog::finished, this, &WindowSettings::onQDialogFinished);
}

QGroupBox* WindowSettings::meterBox()
{
	auto meter_box = new QGroupBox("Meter", m_dialog);

	QWidgetList check_boxes = {
		makeCheckBox(Type::MeterLinePos),
		makeCheckBox(Type::MeterColPos),
		makeCheckBox(Type::MeterLineCount),
		makeCheckBox(Type::MeterWordCount),
		makeCheckBox(Type::MeterCharCount)
	};

	Layout::box(Box::Vertical, meter_box, check_boxes);

	return meter_box;
}

QCheckBox* WindowSettings::makeCheckBox(Type type)
{
	auto setting_name = name(type);
	auto action = m_namesToActions[setting_name];

	auto check_box = new QCheckBox(setting_name, m_dialog);
	check_box->setTristate(false);
	check_box->setChecked(action->isChecked());

	connect(check_box, &QCheckBox::stateChanged, this, [=](int state) {
		action->setChecked(state);
		});

	return check_box;
}

constexpr const char* WindowSettings::name(Type type, Prefix prefix)
{
	auto is_prefix = (prefix == Prefix::Yes);

	switch (type) {
	case Type::MeterLinePos:
		return is_prefix ? PREFIX_METER : "Line position";
	case Type::MeterColPos:
		return is_prefix ? PREFIX_METER : "Column position";
	case Type::MeterLineCount:
		return is_prefix ? PREFIX_METER : "Line count";
	case Type::MeterWordCount:
		return is_prefix ? PREFIX_METER : "Word count";
	case Type::MeterCharCount:
		return is_prefix ? PREFIX_METER : "Character count";
	}

	return "";
}

QString WindowSettings::iniName(Type type, Prefix prefix)
{
	return QString(name(type, prefix)).replace(" ", "_");
}

void WindowSettings::onQActionToggled(bool checked)
{
	auto action = sender_cast(QAction);
	if (!action) return;

	auto data = action->data();
	auto type = data.value<Type>();

	emit settingChecked(checked, type);
}

void WindowSettings::onQDialogFinished()
{
	delete m_dialog;
	m_dialog = nullptr;
}
