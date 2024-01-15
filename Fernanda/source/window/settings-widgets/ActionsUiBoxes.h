#pragma once

#include "../../common/Layout.hpp"

#include <QAction>
#include <QActionGroup>
#include <QCheckBox>
#include <QComboBox>
#include <QDebug>
#include <QGroupBox>
#include <QList>

class ActionsUiBox : public QGroupBox
{
	Q_OBJECT

public:
	enum Align {
		Horizontal,
		Vertical
	};

	ActionsUiBox(Align alignment = Align::Horizontal, QWidget* parent = nullptr);
	virtual ~ActionsUiBox() {}

protected:
	Box boxAlignment() const;

private:
	Align m_alignment;
};

class ActionsChecksBox : public ActionsUiBox
{
	Q_OBJECT

public:
	ActionsChecksBox(QList<QAction*>& actions, Align alignment = Align::Horizontal, QWidget* parent = nullptr);
	~ActionsChecksBox() { qDebug() << __FUNCTION__; }

private:
	void setup(QList<QAction*>& actions);
	QCheckBox* makeCheckBox(QAction* action);
};

class ActionGroupsDropdownsBox : public ActionsUiBox
{
	Q_OBJECT

public:
	ActionGroupsDropdownsBox(QList<QActionGroup*>& groups, Align alignment = Align::Horizontal, QWidget* parent = nullptr);
	~ActionGroupsDropdownsBox() { qDebug() << __FUNCTION__; }

private:
	void setup(QList<QActionGroup*>& groups);
	QComboBox* makeComboBox(QActionGroup* group);
};
