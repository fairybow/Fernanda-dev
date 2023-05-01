#pragma once

#include "../common/Path.hpp"
#include "Settings.hpp"

#include <QCoreApplication>
#include <QDir>
#include <QStandardPaths>
#include <QString>
#include <QVariant>

#include <filesystem>
#include <map>

constexpr char DEFAULT_GROUP[] = "Common";

class User : public QObject
{
	using StdFsPath = std::filesystem::path;

	Q_OBJECT

public:
	inline User(const QString& applicationName = QCoreApplication::applicationName(), QObject* parent = nullptr, const QString& configFileName = "Settings.ini")
		: QObject(parent), m_configFileName(fileName(configFileName))
	{
		auto data_folder_name = "." + applicationName.toLower();
		auto data_folder_path = Path::toStdFs(QDir::homePath()) / Path::toStdFs(data_folder_name);
		m_folders["data"] = data_folder_path;
		m_folders["tempFiles"] = data_folder_path / ".temp";
		m_folders["backup"] = data_folder_path / "backup";
		auto system_documents = Path::toStdFs(QStandardPaths::locate(
			QStandardPaths::DocumentsLocation, nullptr, QStandardPaths::LocateDirectory));
		m_folders["documents"] = system_documents / Path::toStdFs(applicationName);
		for (auto& [key, data_folder] : m_folders)
			Path::make(data_folder);
		connect(QCoreApplication::instance(), &QCoreApplication::aboutToQuit, this, &User::destroyTemp);
	}

	template<typename T>
	inline void save(T value, const QString& valueKey, const QString& groupPrefix = DEFAULT_GROUP)
	{
		Settings::save(m_folders["data"] / m_configFileName,
			groupPrefix, valueKey, QVariant::fromValue(value));
	}

	inline QVariant load(const QString& valueKey, const QString& groupPrefix = DEFAULT_GROUP, QVariant fallback = QVariant())
	{
		return Settings::load(m_folders["data"] / m_configFileName,
			groupPrefix, valueKey, fallback);
	}

	inline QVariant load(const QString& valueKey, QVariant fallback = QVariant())
	{
		return load(valueKey, DEFAULT_GROUP, fallback);
	}

private:
	std::map<QString, StdFsPath> m_folders;
	const StdFsPath m_configFileName;

	inline StdFsPath fileName(const QString& name)
	{
		auto fs_name = Path::toStdFs(name);
		if (!fs_name.has_extension())
			fs_name.replace_extension(".ini");
		return fs_name;
	}

private slots:
	inline void destroyTemp()
	{
		Path::clear(m_folders["tempFiles"], true);
	}
};
