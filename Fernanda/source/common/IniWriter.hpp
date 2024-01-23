#pragma once

#include "Path.hpp"

#include <QAnyStringView>
#include <QDebug>
#include <QObject>
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

	void setIni(const Path& ini)
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

	void saveIn(QAnyStringView prefix, QAnyStringView key, const QVariant& value)
	{
		begin(prefix);
		save(key, value);
		end();
	}

	QVariant load(QAnyStringView key, const QVariant& fallback = QVariant())
	{
		return m_settings->value(key, fallback);
	}

	QVariant loadFrom(QAnyStringView prefix, QAnyStringView key, const QVariant& fallback = QVariant())
	{
		begin(prefix);
		auto value = load(key, fallback);
		end();

		return value;
	}

private:
	Path m_ini;
	QSettings* m_settings;
};
