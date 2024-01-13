#pragma once

#include "../common/WeirdMap.hpp"

#include <QAction>
#include <QActionGroup>
#include <QList>
#include <QString>
#include <QVariant>

template <typename T>
class SettingsMap : public WeirdMap<T, QVariant>
{
public:
	SettingsMap(QObject* parent = nullptr, const QString& name = QString())
		: WeirdMap<T, QVariant>(parent, name, 2)
	{}

	virtual ~SettingsMap()
	{}

	virtual QVariant state(T* item) const = 0;

	virtual void setState(T* item, const QVariant& state) = 0;

	QVariant itemData(T* item) const
	{
		return WeirdMap<T, QVariant>::keyValue(item, 0);
	}

	void setItemData(T* item, const QVariant& data)
	{
		WeirdMap<T, QVariant>::setKeyValue(item, data, 0);
	}

	QVariant fallback(T* item) const
	{
		return WeirdMap<T, QVariant>::keyValue(item, 1);
	}

	void setFallback(T* item, const QVariant& fallback)
	{
		WeirdMap<T, QVariant>::setKeyValue(item, fallback, 1);
	}

	T* add(const QString& name, const QVariant& data, const QVariant& fallback)
	{
		auto item = WeirdMap<T, QVariant>::add(name, data);
		setFallback(item, fallback);

		return item;
	}
};

class ActionsMap : public SettingsMap<QAction>
{
	Q_OBJECT

public:
	using SettingsMap::SettingsMap;

	~ActionsMap()
	{
		qDebug() << __FUNCTION__;
	}

	QVariant state(QAction* action) const override
	{
		return action->isChecked();
	}

	void setState(QAction* action, const QVariant& state) override
	{
		action->setChecked(state.toBool());
	}

	QList<QAction*> actions() const
	{
		return keys();
	}

	QString actionName(QAction* action) const
	{
		return keyName(action);
	}

	void setActionName(QAction* action, const QString& name)
	{
		setKeyName(action, name);
	}

	QVariant actionData(QAction* action) const
	{
		return SettingsMap<QAction>::itemData(action);
	}

	void setActionData(QAction* action, const QVariant& data)
	{
		SettingsMap<QAction>::setItemData(action, data);
	}

	void setAllCheckable(bool checkable)
	{
		for (auto& action : keys())
			action->setCheckable(checkable);
	}

	QAction* add(const QString& name, const QVariant& data, const QVariant& fallback)
	{
		auto action = SettingsMap<QAction>::add(name, data, fallback);
		action->setText(name);
		action->setData(data);

		return action;
	}
};

class ActionGroupsMap : public SettingsMap<QActionGroup>
{
	Q_OBJECT

public:
	using SettingsMap::SettingsMap;

	~ActionGroupsMap()
	{
		qDebug() << __FUNCTION__;
	}

	QVariant state(QActionGroup* group) const override
	{
		return group->checkedAction()->data();
	}

	void setState(QActionGroup* group, const QVariant& state) override
	{
		for (auto& action : group->actions())
			if (action && action->data() == state) {
				action->setChecked(true);

				return;
			}
	}

	QList<QActionGroup*> groups() const
	{
		return keys();
	}

	QString groupName(QActionGroup* group) const
	{
		return keyName(group);
	}

	void setGroupName(QActionGroup* group, const QString& name)
	{
		setKeyName(group, name);
	}

	QVariant groupData(QActionGroup* group) const
	{
		return SettingsMap<QActionGroup>::itemData(group);
	}

	void setGroupData(QActionGroup* group, const QVariant& data)
	{
		SettingsMap<QActionGroup>::setItemData(group, data);
	}

	void setAllExclusive(bool exclusive)
	{
		for (auto& group : keys())
			group->setExclusive(exclusive);
	}
};
