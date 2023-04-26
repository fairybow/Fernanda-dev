#pragma once

#include <QFile>
#include <QString>
#include <QTextStream>

#include <filesystem>

namespace Io
{
	namespace StdFs = std::filesystem;

	inline const QString readFile(StdFs::path filePath)
	{
		QString text;
		QFile file(filePath);
		if (file.open(QFile::ReadOnly | QIODevice::Text)) {
			QTextStream in(&file);
			text = in.readAll();
		}
		return text;
	}

	inline bool writeFile(StdFs::path filePath, QString text)
	{
		//Path::makeParent(filePath);
		QFile file(filePath);
		if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
			QTextStream out(&file);
			out << text;
			return true;
		}
		return false;
	}
}
