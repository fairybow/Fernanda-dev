#pragma once

#include "../common/Layout.hpp"

#include <QAction>
#include <QActionGroup>
#include <QCheckBox>
#include <QComboBox>
#include <QDebug>
#include <QGroupBox>
#include <QList>
#include <QString>

class ActionsUIBox : public QGroupBox
{
	Q_OBJECT

public:
	enum Align {
		Horizontal,
		Vertical
	};

	ActionsUIBox(Align alignment = Align::Horizontal, QWidget* parent = nullptr)
		: QGroupBox(parent), m_alignment(alignment)
	{}

	virtual ~ActionsUIBox() noexcept
	{
		qDebug() << __FUNCTION__;
	}

protected:
	Box boxAlignment() const
	{
		return (m_alignment == Align::Horizontal) ? Box::Horizontal : Box::Vertical;
	}

private:
	Align m_alignment;
};

class ActionsChecksBox : public ActionsUIBox
{
	Q_OBJECT

public:
	ActionsChecksBox(QList<QAction*>& actions, Align alignment = Align::Horizontal, QWidget* parent = nullptr)
		: ActionsUIBox(alignment, parent)
	{
		setup(actions);
	}

private:
	void setup(QList<QAction*>& actions)
	{
		QWidgetList check_boxes;

		for (auto& action : actions)
			check_boxes << makeCheckBox(action);

		Layout::box(boxAlignment(), this, check_boxes);
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
};

class ActionGroupsDropdownsBox : public ActionsUIBox
{
	Q_OBJECT

public:
	ActionGroupsDropdownsBox(QList<QActionGroup*>& groups, Align alignment = Align::Horizontal, QWidget* parent = nullptr)
		: ActionsUIBox(alignment, parent)
	{
		setup(groups);
	}

private:
	void setup(QList<QActionGroup*>& groups)
	{
		QWidgetList combo_boxes;

		for (auto& group : groups)
			combo_boxes << makeComboBox(group);

		Layout::box(boxAlignment(), this, combo_boxes);
	}

	QComboBox* makeComboBox(QActionGroup* group)
	{
		auto combo_box = new QComboBox(this);

		// Add items from group and set checkedAction to currentIndex

		connect(combo_box, &QComboBox::currentTextChanged, this, [=](const QString& text) {
			for (auto& action : group->actions())
				if (action->text() == text) {
					action->setChecked(true);

					return;
				}
			});

		return combo_box;
	}
};
