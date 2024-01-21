#pragma once

#include "../../common/Layout.hpp"

//#include <QAction>
#include <QActionGroup>
#include <QCheckBox>
#include <QComboBox>
#include <QDebug>
#include <QGroupBox>
#include <QList>

class ActionGroupUiBox : public QGroupBox
{
	Q_OBJECT

public:
	enum Align {
		Horizontal,
		Vertical
	};

	ActionGroupUiBox(Align alignment = Align::Horizontal, QWidget* parent = nullptr);
	virtual ~ActionGroupUiBox() {}

protected:
	Box boxAlignment() const;

private:
	Align m_alignment;
};

class ActionGroupChecksBox : public ActionGroupUiBox
{
	Q_OBJECT

public:
	ActionGroupChecksBox(QActionGroup* actionGroup, Align alignment = Align::Horizontal, QWidget* parent = nullptr);
	~ActionGroupChecksBox() { qDebug() << __FUNCTION__; }

private:
	void setup(QActionGroup* actionGroup);
	QCheckBox* makeCheckBox(QAction* action);
};

class ActionGroupDropdownsBox : public ActionGroupUiBox
{
	Q_OBJECT

public:
	ActionGroupDropdownsBox(QList<QActionGroup*>& actionGroups, Align alignment = Align::Horizontal, QWidget* parent = nullptr);
	~ActionGroupDropdownsBox() { qDebug() << __FUNCTION__; }

private:
	void setup(QList<QActionGroup*>& actionGroups);
	QComboBox* makeComboBox(QActionGroup* group);
};
