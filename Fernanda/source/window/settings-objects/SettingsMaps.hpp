#pragma once

#include "../../common/WeirdMap.hpp"

#include <QAction>
#include <QActionGroup>
#include <QList>
#include <QString>
#include <QVariant>

template <typename TKey>
class SettingsMap : public WeirdMap<TKey, QVariant>
{
public:
	SettingsMap(QObject* parent = nullptr, const QString& name = QString())
		: WeirdMap<TKey, QVariant>(parent, name, 2)
	{}

	virtual ~SettingsMap()
	{}

	virtual QVariant state(TKey key) const = 0;

	virtual void setState(TKey key, const QVariant& state) = 0;

	QVariant dataOf(const TKey& key) const
	{
		return WeirdMap<TKey, QVariant>::valueOf(key, 0);
	}

	void setDataOf(const TKey& key, const QVariant& data)
	{
		WeirdMap<TKey, QVariant>::setValueOf(key, data, 0);
	}

	QVariant fallback(const TKey& key) const
	{
		return WeirdMap<TKey, QVariant>::valueOf(key, 1);
	}

	void setFallback(const TKey& key, const QVariant& fallback)
	{
		WeirdMap<TKey, QVariant>::setValueOf(key, fallback, 1);
	}

	TKey add(const TKey& key, const QString& name, const QVariant& data, const QVariant& fallback)
	{
		WeirdMap<TKey, QVariant>::add(key, name, data);
		setFallback(key, fallback);

		return key;
	}
};

class ActionsMap : public SettingsMap<QAction*>
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

	void setNameOf(QAction* action, const QString& name)
	{
		action->setText(name);

		SettingsMap<QAction*>::setNameOf(action, name);
	}

	void setDataOf(QAction* action, const QVariant& data)
	{
		action->setData(data);

		SettingsMap<QAction*>::setDataOf(action, data);
	}

	QList<QAction*> actions() const
	{
		return keys();
	}

	void setAllCheckable(bool checkable)
	{
		for (auto& action : actions())
			action->setCheckable(checkable);
	}

	QAction* add(const QString& name, const QVariant& data, const QVariant& fallback)
	{
		auto action = new QAction(this);
		SettingsMap<QAction*>::add(action, name, data, fallback);
		action->setText(name);
		action->setData(data);

		return action;
	}
};

class ActionGroupsMap : public SettingsMap<QActionGroup*>
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

	void setAllExclusive(bool exclusive)
	{
		for (auto& group : groups())
			group->setExclusive(exclusive);
	}

	QActionGroup* add(const QString& name, const QVariant& data, const QVariant& fallback)
	{
		auto group = new QActionGroup(this);
		SettingsMap<QActionGroup*>::add(group, name, data, fallback);

		return group;
	}
};
