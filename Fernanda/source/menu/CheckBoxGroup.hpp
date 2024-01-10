#pragma once

#include "../common/Layout.hpp"
#include "ActionGroup.hpp"

#include <QAction>
#include <QCheckBox>
#include <QDebug>
#include <QGroupBox>
#include <QList>

class CheckBoxGroup : public QGroupBox
{
	Q_OBJECT

public:
	enum Align {
		Horizontal,
		Vertical
	};

	CheckBoxGroup(ActionGroup* group, Align alignment = Align::Horizontal, QWidget* parent = nullptr)
		: QGroupBox(parent)
	{
		setup(group, alignment);
	}

	~CheckBoxGroup()
	{
		qDebug() << __FUNCTION__;
	}

private:
	void setup(ActionGroup* group, Align alignment)
	{
		setTitle(group->name());

		QWidgetList check_boxes;

		for (auto& action : group->actions())
			check_boxes << makeCheckBox(action);

		Layout::box(boxAlignment(alignment), this, check_boxes);
	}

	QCheckBox* makeCheckBox(QAction* action)
	{
		auto check_box = new QCheckBox(action->text(), this);
		check_box->setTristate(false);
		check_box->setChecked(action->isChecked());

		connect(check_box, &QCheckBox::stateChanged, this, [=](int state) {
			action->setChecked(state);
			});

		return check_box;
	}

	Box boxAlignment(Align alignment) const
	{
		return (alignment == Align::Horizontal) ? Box::Horizontal : Box::Vertical;
	}
};
