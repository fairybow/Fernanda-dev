#pragma once

#include "../common/Path.hpp"

#include <QSettings>

#include <memory>

class Settings
{
	using StdFsPath = std::filesystem::path;

public:
	static inline void save(StdFsPath config, const QString& groupPrefix, const QString& valueKey, QVariant value)
	{
		auto ini = iniFile(config, groupPrefix);
		ini->setValue(valueKey, value);
		ini->endGroup();
	}

	static inline QVariant load(StdFsPath config, const QString& groupPrefix, const QString& valueKey, QVariant fallback = QVariant())
	{
		auto ini = iniFile(config, groupPrefix);
		auto value = ini->value(valueKey, fallback);
		ini->endGroup();
		return value;
	}

private:
	static inline std::unique_ptr<QSettings> iniFile(StdFsPath config, const QString& groupPrefix)
	{
		auto ini = std::make_unique<QSettings>(
			Path::toQString(config), QSettings::IniFormat);
		ini->beginGroup(groupPrefix);
		return ini;
	}
};
