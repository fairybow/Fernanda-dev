#include "ActionsUiBoxes.h"

#include <QString>

ActionsUiBox::ActionsUiBox(Align alignment, QWidget* parent)
	: QGroupBox(parent), m_alignment(alignment)
{}

Box ActionsUiBox::boxAlignment() const
{
	return (m_alignment == Align::Horizontal) ? Box::Horizontal : Box::Vertical;
}

ActionsChecksBox::ActionsChecksBox(QList<QAction*>& actions, Align alignment, QWidget* parent)
	: ActionsUiBox(alignment, parent)
{
	setup(actions);
}

void ActionsChecksBox::setup(QList<QAction*>& actions)
{
	QWidgetList check_boxes;

	for (auto& action : actions)
		check_boxes << makeCheckBox(action);

	auto layout = Layout::box(boxAlignment(), this, check_boxes);
	layout->setContentsMargins(5, 5, 5, 5);
}

QCheckBox* ActionsChecksBox::makeCheckBox(QAction* action)
{
	auto check_box = new QCheckBox(action->text(), this);
	check_box->setTristate(false);
	check_box->setChecked(action->isChecked());

	connect(check_box, &QCheckBox::stateChanged, this, [=](int state) {
		action->setChecked(state);
		});

	return check_box;
}

ActionGroupsDropdownsBox::ActionGroupsDropdownsBox(QList<QActionGroup*>& groups, Align alignment, QWidget* parent)
	: ActionsUiBox(alignment, parent)
{
	setup(groups);
}

void ActionGroupsDropdownsBox::setup(QList<QActionGroup*>& groups)
{
	QWidgetList combo_boxes;

	for (auto& group : groups)
		combo_boxes << makeComboBox(group);

	auto layout = Layout::box(boxAlignment(), this, combo_boxes);
	layout->setContentsMargins(5, 5, 5, 5);
}

QComboBox* ActionGroupsDropdownsBox::makeComboBox(QActionGroup* group)
{
	auto combo_box = new QComboBox(this);

	// Add items from group and set checkedAction to currentIndex

	// Change the connection. Set pointer to action as data and if data equals action on `currentIndexChanged`, set action checked.

	connect(combo_box, &QComboBox::currentTextChanged, this, [=](const QString& text) {
		for (auto& action : group->actions())
			if (action->text() == text) {
				action->setChecked(true);

				return;
			}
		});

	return combo_box;
}
