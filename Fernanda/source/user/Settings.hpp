#pragma once

#include "../common/Path.hpp"

#include <QSettings>
#include <QString>
#include <QVariant>

#include <filesystem>
#include <memory>

class Settings
{
	using StdFsPath = std::filesystem::path;

	template<typename T>
	using IsNotStdFs = std::negation<std::is_same<T, StdFsPath>>;

public:
	template<typename T>
	static inline typename std::enable_if<IsNotStdFs<T>::value, void>::type
		save(StdFsPath config, const QString& groupPrefix, const QString& valueKey, T value)
	{
		auto ini = iniFile(config, groupPrefix);
		ini->setValue(valueKey, QVariant::fromValue(value));
		ini->endGroup();
	}

	template<typename T>
	static inline T load(StdFsPath config, const QString& groupPrefix, const QString& valueKey, T fallbackValue = T())
	{
		auto ini = iniFile(config, groupPrefix);
		auto variant_value = ini->value(valueKey, QVariant::fromValue(fallbackValue));
		ini->endGroup();
		return variant_value.value<T>();
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
