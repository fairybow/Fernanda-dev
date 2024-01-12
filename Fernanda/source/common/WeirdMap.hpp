#pragma once

#include <QDebug>
#include <QList>
#include <QMap>
#include <QSet>
#include <QString>
#include <QVariant>

#include <type_traits>

template <typename T, typename U>
class WeirdMap : public QObject
{
	static_assert(std::is_base_of<QObject, T>::value, "T must derive from QObject");

public:
	WeirdMap(QObject* parent, const QString& name, int itemDataSlots = 1)
		: QObject(parent), m_name(name)
	{
		setup(itemDataSlots);
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
		return m_itemsOrdered;
	}

	bool contains(T* item) const
	{
		return m_items.contains(item);
	}

	T* itemWith(const U& data) const
	{
		for (auto& map : m_itemData)
			for (auto it = map.begin(); it != map.end(); ++it)
				if (it.value() == data)
					return it.key();

		return nullptr;
	}

	QList<T*> itemsWith(const U& data) const
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

	U data() const
	{
		return m_data;
	}

	void setData(const U& data)
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
			auto index = bound(itemDataSlot);

			return m_itemData[index][item];
		}

		return U();
	}

	void setItemData(T* item, const U& data, int itemDataSlot = 0)
	{
		if (!contains(item)) return;

		auto index = bound(itemDataSlot);
		m_itemData[index][item] = data;
	}

	T* add(T* item)
	{
		if (contains(item))
			return item;

		item->setParent(this);
		m_items << item;
		m_itemsOrdered << item;

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

	T* add(const QString& name, const U& data, int itemDataSlot = 0)
	{
		auto item = add(name);
		setItemData(item, data, itemDataSlot);

		return item;
	}

private:
	QString m_name;
	U m_data;
	QSet<T*> m_items;
	QList<T*> m_itemsOrdered;
	QMap<T*, QString> m_itemNames;
	QList<QMap<T*, U>> m_itemData; // Switch to QMap<T*, QList<U>>?

	void setup(int itemDataSlots)
	{
		auto max_slots = qBound(1, itemDataSlots, 5);

		m_itemData.resize(max_slots);
	}

	constexpr const int bound(int itemDataSlot) const
	{
		int max_item_data_index = m_itemData.size() - 1;

		return qBound(0, itemDataSlot, max_item_data_index);
	}
};
