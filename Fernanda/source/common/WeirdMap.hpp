#pragma once

#include <QDebug>
#include <QList>
#include <QMap>
#include <QSet>
#include <QString>
#include <QVariant>

#include <type_traits>

template <typename T>
class WeirdMap : public QObject
{
	static_assert(std::is_base_of<QObject, T>::value, "T must derive from QObject");

public:
	WeirdMap(QObject* parent, const QString& name, int itemDataSlots = 1)
		: QObject(parent), m_name(name)
	{
		m_itemData.resize(qBound(1, itemDataSlots, 5));
	}

	WeirdMap(QObject* parent = nullptr, int itemDataSlots = 1)
		: WeirdMap(parent, QString(), itemDataSlots)
	{}

	virtual ~WeirdMap() noexcept
	{
		qDebug() << __FUNCTION__;
	}

	QList<T*> items() const
	{
		return m_itemsOrder;
	}

	bool contains(T* item) const
	{
		return m_items.contains(item);
	}

	T* itemWith(const QVariant& data) const
	{
		for (auto& map : m_itemData)
			for (auto it = map.begin(); it != map.end(); ++it)
				if (it.value() == data)
					return it.key();

		return nullptr;
	}

	QList<T*> itemsWith(const QVariant& data) const
	{
		QList<T*> items;

		for (auto& map : m_itemData)
			for (auto it = map.begin(); it != map.end(); ++it)
				if (it.value() == data) {
					auto key = it.key();

					if (!items.contains(key))
						items << key;
				}

		return items;
	}

	QString name() const
	{
		return m_name;
	}

	void setName(const QString& name)
	{
		m_name = name;
	}

	QVariant data() const
	{
		return m_data;
	}

	void setData(const QVariant& data)
	{
		m_data = data;
	}

	QString itemName(T* item) const
	{
		if (contains(item))
			return m_itemNames[item];

		return QString();
	}

	void setItemName(T* item, const QString& name)
	{
		if (contains(item))
			m_itemNames[item] = name;
	}

	QVariant itemData(T* item, int itemDataSlot = 0) const
	{
		if (contains(item)) {
			auto index = qBound(0, itemDataSlot, maxItemDataIndex());

			return m_itemData[index][item];
		}

		return QVariant();
	}

	void setItemData(T* item, const QVariant& data, int itemDataSlot = 0)
	{
		if (!contains(item)) return;

		auto index = qBound(0, itemDataSlot, maxItemDataIndex());
		m_itemData[index][item] = data;
	}

	T* add(T* item)
	{
		item->setParent(this);
		m_items << item;
		m_itemsOrder << item;

		return item;
	}

	T* add()
	{
		return add(new T(nullptr));
	}

	T* add(const QString& name)
	{
		auto item = add();
		setItemName(item, name);

		return item;
	}

	T* add(const QString& name, const QVariant& data, int itemDataSlot = 0)
	{
		auto item = add(name);
		setItemData(item, data, itemDataSlot);

		return item;
	}

private:
	QString m_name;
	QVariant m_data;
	QSet<T*> m_items;
	QList<T*> m_itemsOrder;
	QMap<T*, QString> m_itemNames;
	QList<QMap<T*, QVariant>> m_itemData; // Switch to QMap<T*, QList<QVariant>>?

	int maxItemDataIndex() const
	{
		return m_itemData.size() - 1;
	}
};
