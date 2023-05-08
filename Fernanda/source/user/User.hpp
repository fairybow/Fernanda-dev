#pragma once

#include "../common/Path.hpp"
#include "Settings.hpp"

#include <QCoreApplication>
#include <QDir>
#include <QStandardPaths>
#include <QString>

#include <filesystem>
#include <map>

constexpr char BACKUP_NAME[] = "backup";
constexpr char DATA_NAME[] = "data";
constexpr char TEMP_NAME[] = "tempFiles";

class User : public QObject
{
	using StdFsPath = std::filesystem::path;

	Q_OBJECT

public:
	User(const QString& applicationName = QCoreApplication::applicationName(), QObject* parent = nullptr, const QString& configFileName = "Settings.ini")
		: QObject(parent), m_configFileName(fileName(configFileName))
	{
		auto data_folder_name = "." + applicationName.toLower();
		auto data_folder_path = Path::toStdFs(QDir::homePath()) / Path::toStdFs(data_folder_name);
		m_folders[DATA_NAME] = data_folder_path;
		m_folders[TEMP_NAME] = data_folder_path / ".temp";
		m_folders[BACKUP_NAME] = data_folder_path / BACKUP_NAME;
		auto system_documents = Path::toStdFs(QStandardPaths::locate(
			QStandardPaths::DocumentsLocation, nullptr, QStandardPaths::LocateDirectory));
		m_folders["documents"] = system_documents / Path::toStdFs(applicationName);
		for (auto& [key, data_folder] : m_folders)
			Path::make(data_folder);
		connect(QCoreApplication::instance(), &QCoreApplication::aboutToQuit, this, &User::destroyTemp);
	}

	template<typename T>
	void save(T value, const QString& valueKey, const QString& groupPrefix = QString())
	{
		Settings::save(m_folders[DATA_NAME] / m_configFileName,
			groupPrefix, valueKey, value);
	}

	template<typename T>
	void save(T value, const QString& valueKey, QObject* namedObject)
	{
		save(value, valueKey, namedObject->objectName());
	}

	template<typename T>
	T load(const QString& valueKey, const QString& groupPrefix = QString(), T fallbackValue = T())
	{
		return Settings::load(m_folders[DATA_NAME] / m_configFileName,
			groupPrefix, valueKey, fallbackValue);
	}

	template<typename T>
	T load(const QString& valueKey, QObject* namedObject, T fallbackValue = T())
	{
		return load(valueKey, namedObject->objectName(), fallbackValue);
	}

	/*template<typename T>
	T load(const QString& valueKey, T fallbackValue = T())
	{
		return load(valueKey, QString(), fallbackValue);
	}*/

	StdFsPath dataFolder() { return m_folders[DATA_NAME]; }

private:
	std::map<QString, StdFsPath> m_folders;
	const StdFsPath m_configFileName;

	StdFsPath fileName(const QString& name)
	{
		auto fs_name = Path::toStdFs(name);
		if (!fs_name.has_extension())
			fs_name.replace_extension(".ini");
		return fs_name;
	}

private slots:
	void destroyTemp()
	{
		Path::clear(m_folders[TEMP_NAME], true);
	}
};
