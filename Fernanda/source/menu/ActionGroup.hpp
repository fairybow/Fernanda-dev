#pragma once

#include <QAction>
#include <QActionGroup>
#include <QDebug>
#include <QList>
#include <QString>
#include <QVariant>

class ActionGroup : public QActionGroup
{
	Q_OBJECT

public:
	ActionGroup(QObject* parent = nullptr, const QString& name = QString())
		: QActionGroup(parent), m_name(name)
	{}

	~ActionGroup()
	{
		qDebug() << __FUNCTION__;
	}

	QVariant data() const
	{
		return m_data;
	}

	void setData(const QVariant& data)
	{
		m_data = data;
	}

	QString name() const
	{
		return m_name;
	}

	void setName(const QString& name)
	{
		m_name = name;
	}

	void setAllCheckable(bool checkable)
	{
		for (auto& action : actions())
			action->setCheckable(checkable);
	}

	QList<QAction*>& addActions(QList<QAction*>& actions)
	{
		for (auto& action : actions)
			addAction(action);

		return actions;
	}

private:
	QVariant m_data;
	QString m_name;
};
