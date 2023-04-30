#pragma once

#include "../common/Path.hpp"

#include <QSettings>

class Settings : public QObject
{
	using StdFsPath = std::filesystem::path;

public:
	inline Settings(QObject* parent = nullptr)
		: QObject(parent) {}

	inline void saveConfig(StdFsPath configFile, QVariant value)
	{
		qDebug() << configFile << value;
	}
};
