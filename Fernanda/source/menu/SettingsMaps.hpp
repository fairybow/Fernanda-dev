#pragma once

#include "../common/WeirdMap.hpp"

#include <QAction>
#include <QActionGroup>
#include <QList>
#include <QString>
#include <QVariant>

template <typename T>
class SettingsMaps : public WeirdMap<T>
{
public:
	SettingsMaps(QObject* parent = nullptr, const QString& name = QString())
		: WeirdMap<T>(parent, name, 2)
	{}

	QVariant itemData(T* item) const
	{
		return WeirdMap<T>::itemData(item, 0);
	}

	void setItemData(T* item, const QVariant& data)
	{
		WeirdMap<T>::setItemData(item, data, 0);
	}

	QVariant fallback(T* item) const
	{
		return WeirdMap<T>::itemData(item, 1);
	}

	void setFallback(T* item, const QVariant& fallback)
	{
		WeirdMap<T>::setItemData(item, fallback, 1);
	}

	T* add(const QString& name, const QVariant& data, const QVariant& fallback)
	{
		auto item = WeirdMap<T>::add(name, data);
		setFallback(item, fallback);

		return item;
	}
};

class ActionSet : public SettingsMaps<QAction>
{
	Q_OBJECT

public:
	using SettingsMaps::SettingsMaps;

	QList<QAction*> actions() const
	{
		return items();
	}

	QString actionName(QAction* action) const
	{
		return itemName(action);
	}

	void setActionName(QAction* action, const QString& name)
	{
		setItemName(action, name);
	}

	QVariant actionData(QAction* action) const
	{
		return SettingsMaps<QAction>::itemData(action);
	}

	void setActionData(QAction* action, const QVariant& data)
	{
		SettingsMaps<QAction>::setItemData(action, data);
	}

	void setAllCheckable(bool checkable)
	{
		for (auto& action : items())
			action->setCheckable(checkable);
	}

	QAction* add(const QString& name, const QVariant& data, const QVariant& fallback)
	{
		auto action = SettingsMaps<QAction>::add(name, data, fallback);
		action->setText(name);
		action->setData(data);

		return action;
	}
};

class ActionGroupSet : public SettingsMaps<QActionGroup>
{
	Q_OBJECT

public:
	using SettingsMaps::SettingsMaps;

	QList<QActionGroup*> groups() const
	{
		return items();
	}

	QString groupName(QActionGroup* group) const
	{
		return itemName(group);
	}

	void setGroupName(QActionGroup* group, const QString& name)
	{
		setItemName(group, name);
	}

	QVariant groupData(QActionGroup* group) const
	{
		return SettingsMaps<QActionGroup>::itemData(group);
	}

	void setGroupData(QActionGroup* group, const QVariant& data)
	{
		SettingsMaps<QActionGroup>::setItemData(group, data);
	}

	void setAllExclusive(bool exclusive)
	{
		for (auto& group : items())
			group->setExclusive(exclusive);
	}
};
