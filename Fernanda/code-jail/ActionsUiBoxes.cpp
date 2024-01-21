#include "ActionsUiBoxes.h"

#include <QString>

ActionGroupUiBox::ActionGroupUiBox(Align alignment, QWidget* parent)
	: QGroupBox(parent), m_alignment(alignment)
{}

Box ActionGroupUiBox::boxAlignment() const
{
	return (m_alignment == Align::Horizontal) ? Box::Horizontal : Box::Vertical;
}

ActionGroupChecksBox::ActionGroupChecksBox(QActionGroup* actionGroup, Align alignment, QWidget* parent)
	: ActionGroupUiBox(alignment, parent)
{
	setup(actionGroup);
}

void ActionGroupChecksBox::setup(QActionGroup* actionGroup)
{
	QWidgetList check_boxes;

	//for (auto& action : actionGroup)
		//check_boxes << makeCheckBox(action);

	//auto layout = Layout::box(boxAlignment(), this, check_boxes);
	//layout->setContentsMargins(5, 5, 5, 5);
}

QCheckBox* ActionGroupChecksBox::makeCheckBox(QAction* action)
{
	auto check_box = new QCheckBox(action->text(), this);
	check_box->setTristate(false);
	check_box->setChecked(action->isChecked());

	connect(check_box, &QCheckBox::stateChanged, this, [=](int state) {
		action->setChecked(state);
		});

	return check_box;
}

ActionGroupDropdownsBox::ActionGroupDropdownsBox(QList<QActionGroup*>& actionGroups, Align alignment, QWidget* parent)
	: ActionGroupUiBox(alignment, parent)
{
	setup(actionGroups);
}

void ActionGroupDropdownsBox::setup(QList<QActionGroup*>& actionGroups)
{
	QWidgetList combo_boxes;

	for (auto& group : actionGroups)
		combo_boxes << makeComboBox(group);

	auto layout = Layout::box(boxAlignment(), this, combo_boxes);
	layout->setContentsMargins(5, 5, 5, 5);
}

QComboBox* ActionGroupDropdownsBox::makeComboBox(QActionGroup* group)
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
