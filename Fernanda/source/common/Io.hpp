#pragma once

#include "Path.hpp"

#include <QFile>
#include <QIODevice>
#include <QString>
#include <QTextStream>

namespace Io
{
	enum class CreateDirs {
		No,
		Yes
	};

	inline const QString read(const Path& path)
	{
		QString text;
		QFile file(path.toStd());

		if (file.open(QFile::ReadOnly | QIODevice::Text)) {
			QTextStream in(&file);
			text = in.readAll();
		}

		return text;
	}

	inline bool write(const Path& path, QString text = QString(), CreateDirs createDirectories = CreateDirs::Yes)
	{
		if (createDirectories == CreateDirs::Yes) {
			auto parent_path = path.parent();

			if (!parent_path.isValid())
				Path::createDirs(parent_path);
		}

		QFile file(path.toStd());

		if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
			QTextStream out(&file);
			out << text;

			return true;
		}

		return false;
	}
}
