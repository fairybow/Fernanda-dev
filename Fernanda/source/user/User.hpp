#pragma once

#include "../common/Path.hpp"

#include <QStandardPaths>

class User
{
public:
	inline User(const QString& name = "User")
	{
		auto data_folder = "." + name.toLower();
		auto user_data = Path::toStdFs(QDir::homePath()) / Path::toStdFs(data_folder);
		auto temp_folder = user_data / ".temp";
		auto backup = user_data / "backup";
		auto documents = Path::toStdFs(QStandardPaths::locate(QStandardPaths::DocumentsLocation, nullptr, QStandardPaths::LocateDirectory));
		auto user_documents = documents / Path::toStdFs(name);
		for (const auto& data_folder : { user_data, temp_folder, backup, user_documents })
			Path::makeDirectories(data_folder);
	}

private:
	//
};
