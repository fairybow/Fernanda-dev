#pragma once

#include <QDebug>
#include <QList>
#include <QMap>
#include <QObject>
#include <QSet>
#include <QString>
#include <QVariant>

#include <type_traits>

template <typename TKey, typename TValue, typename TMapData = QVariant>
class WeirdMap : public QObject
{
	static_assert(std::is_base_of<QObject, TKey>::value, "TKey must derive from QObject");

public:
	WeirdMap(QObject* parent, const QString& name, int keyValueSlots = 1)
		: QObject(parent), m_name(name)
	{
		setup(keyValueSlots);
	}

	WeirdMap(QObject* parent = nullptr, int keyValueSlots = 1)
		: WeirdMap(parent, QString(), keyValueSlots)
	{}

	virtual ~WeirdMap()
	{}

	QList<TKey*> keys() const
	{
		return m_keysOrdered;
	}

	bool contains(TKey* key) const
	{
		return m_keys.contains(key);
	}

	TKey* keyWith(const TValue& value) const
	{
		for (auto& map : m_keyValues)
			for (auto it = map.begin(); it != map.end(); ++it)
				if (it.value() == value)
					return it.key();

		return nullptr;
	}

	QList<TKey*> keysWith(const TValue& value) const
	{
		QList<TKey*> keys;

		for (auto& map : m_keyValues)
			for (auto it = map.begin(); it != map.end(); ++it)
				if (it.value() == value) {
					auto key = it.key();

					if (!keys.contains(key))
						keys << key;
				}

		return keys;
	}

	QString name() const
	{
		return m_name;
	}

	void setName(const QString& name)
	{
		m_name = name;
	}

	TMapData data() const
	{
		return m_data;
	}

	void setData(const TMapData& data)
	{
		m_data = data;
	}

	QString keyName(TKey* key) const
	{
		if (contains(key))
			return m_keyNames[key];

		return QString();
	}

	void setKeyName(TKey* key, const QString& name)
	{
		if (contains(key))
			m_keyNames[key] = name;
	}

	TValue keyValue(TKey* key, int keyValueSlot = 0) const
	{
		if (contains(key)) {
			auto index = bound(keyValueSlot);

			return m_keyValues[index][key];
		}

		return TValue();
	}

	void setKeyValue(TKey* key, const TValue& value, int keyValueSlot = 0)
	{
		if (!contains(key)) return;

		auto index = bound(keyValueSlot);
		m_keyValues[index][key] = value;
	}

	TKey* add(TKey* key)
	{
		if (contains(key))
			return key;

		key->setParent(this);
		m_keys << key;
		m_keysOrdered << key;

		return key;
	}

	TKey* add()
	{
		return add(new TKey(nullptr));
	}

	TKey* add(const QString& name)
	{
		auto key = add();
		setKeyName(key, name);

		return key;
	}

	TKey* add(const QString& name, const TValue& value, int keyValueSlot = 0)
	{
		auto key = add(name);
		setKeyValue(key, value, keyValueSlot);

		return key;
	}

private:
	QString m_name;
	TMapData m_data;
	QSet<TKey*> m_keys;
	QList<TKey*> m_keysOrdered;
	QMap<TKey*, QString> m_keyNames;
	QList<QMap<TKey*, TValue>> m_keyValues; // Switch to `QMap<TKey*, QList<TValue>>`?

	void setup(int keyValueSlots)
	{
		auto max_slots = qBound(1, keyValueSlots, 5);

		m_keyValues.resize(max_slots);
	}

	constexpr const int bound(int keyValueSlot) const
	{
		int max_value_index = m_keyValues.size() - 1;

		return qBound(0, keyValueSlot, max_value_index);
	}
};
