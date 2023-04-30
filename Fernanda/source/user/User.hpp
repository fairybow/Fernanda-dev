#pragma once

#include "../common/Path.hpp"
#include "Settings.hpp"

#include <QCoreApplication>
#include <QMap>
#include <QStandardPaths>
#include <QString>

class User : public QObject
{
	using StdFsPath = std::filesystem::path;

	Q_OBJECT

public:
	inline User(const QString& applicationName = QCoreApplication::applicationName(), QObject* parent = nullptr)
		: QObject(parent)
	{
		auto data_folder_name = "." + applicationName.toLower();
		auto data_folder_path = Path::toStdFs(QDir::homePath()) / Path::toStdFs(data_folder_name);
		m_folders["data"] = data_folder_path;
		m_folders["tempFiles"] = data_folder_path / ".temp";
		m_folders["backup"] = data_folder_path / "backup";
		auto system_documents = Path::toStdFs(QStandardPaths::locate(
			QStandardPaths::DocumentsLocation, nullptr, QStandardPaths::LocateDirectory));
		m_folders["documents"] = system_documents / Path::toStdFs(applicationName);
		for (auto& data_folder : m_folders.values())
			Path::make(data_folder);
		connect(QCoreApplication::instance(), &QCoreApplication::aboutToQuit, this, &User::destroyTemp);
	}

	template<typename T>
	inline void save(T value)
	{
		m_settings->saveConfig(
			m_folders["data"] / "Settings.ini", QVariant::fromValue(value));
	}

private:
	Settings* m_settings = new Settings(this);
	QMap<QString, StdFsPath> m_folders;

private slots:
	inline void destroyTemp()
	{
		Path::clear(m_folders["tempFiles"], true);
	}
};
