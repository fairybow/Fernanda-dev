#pragma once

#include "../common/Path.hpp"

#include <QAnyStringView>
#include <QDebug>
#include <QSettings>
#include <QString>
#include <QVariant>

#include <functional>

class IniWriter : public QObject
{
	Q_OBJECT

public:
	IniWriter(const Path& ini, QObject* parent = nullptr)
		: QObject(parent), m_ini(ini)
	{
		Path::createDirs(ini.parent());

		m_settings = new QSettings(ini.toQString(), QSettings::IniFormat, this);
	}

	~IniWriter()
	{
		qDebug() << __FUNCTION__;
	}

	Path ini()
	{
		return m_ini;
	}

	Path setIni(const Path& ini)
	{
		m_ini = ini;
	}

	void begin(QAnyStringView prefix)
	{
		m_settings->beginGroup(prefix);
	}

	void end()
	{
		m_settings->endGroup();
	}

	void save(QAnyStringView key, const QVariant& value)
	{
		m_settings->setValue(key, value);
	}

	template <typename T>
	T load(QAnyStringView key, QVariant fallback = QVariant())
	{
		auto key_value = m_settings->value(key, fallback);

		if (!key_value.isValid())
			return fallback.value<T>();

		return key_value.value<T>();
	}

	template <typename T>
	T load(QAnyStringView key, T fallback)
	{
		return load<T>(key, QVariant::fromValue<T>(fallback));
	}

	template <typename T>
	T load(QAnyStringView key, std::function<void(T)> setter, T fallback = T())
	{
		auto value = load<T>(key, fallback);
		setter(value);

		return value;
	}

private:
	Path m_ini;
	QSettings* m_settings;
};
