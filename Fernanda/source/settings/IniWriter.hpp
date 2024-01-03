#pragma once

#include "../common/Path.hpp"

#include <QDebug>
#include <QSettings>
#include <QString>
#include <QVariant>

class IniWriter : public QObject
{
	Q_OBJECT

public:
	IniWriter(const Path& ini, QObject* parent = nullptr)
		: QObject(parent), m_ini(ini)
	{
		Path::createDirs(ini.parent());

		m_settings = new QSettings(ini.toQString(), QSettings::IniFormat);
	}

	~IniWriter()
	{
		qDebug() << __FUNCTION__;

		delete m_settings;
	}

	Path ini()
	{
		return m_ini;
	}

	template<typename T>
	void save(const QString& prefix, const QString& key, T value)
	{
		m_settings->beginGroup(prefix);
		m_settings->setValue(key, QVariant::fromValue(value));
		m_settings->endGroup();
	}

	template<typename T>
	T load(const QString& prefix, const QString& key, T fallback = T())
	{
		m_settings->beginGroup(prefix);
		auto variant_value = m_settings->value(key, QVariant::fromValue(fallback));
		m_settings->endGroup();

		return variant_value.value<T>();
	}

private:
	Path m_ini;
	QSettings* m_settings;
};
