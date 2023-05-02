#pragma once

#include "../common/Path.hpp"

#include <QActionGroup>
#include <QDirIterator>
#include <QStringList>
#include <QVector>

#include <algorithm>

namespace ResourceGroups
{
	using StdFsPath = std::filesystem::path;

	namespace
	{
		inline void checkExtensions(QStringList& extensions)
		{
			for (auto& extension : extensions)
				if (!extension.startsWith("*"))
					extension = "*" + extension;
		}

		inline void abcByFileName(QStringList& paths)
		{
			std::sort(paths.begin(), paths.end(), [](auto& lhs, auto& rhs) {
				return Path::name(lhs) < Path::name(rhs);
				});
		}

		inline QStringList gather(const QStringList& qrcPaths, const QStringList& extensions)
		{
			QStringList entries;
			for (auto& qrc_path : qrcPaths) {
				QDirIterator it(qrc_path, extensions, QDir::Files, QDirIterator::Subdirectories);
				while (it.hasNext()) {
					it.next();
					entries << it.filePath();
				}
			}
			return entries;
		}
	}

	inline QActionGroup* make(const QStringList& qrcPaths, QStringList extensions,
		QVector<StdFsPath> systemPaths = {}, QWidget * parent = nullptr)
	{
		auto group = new QActionGroup(parent);
		checkExtensions(extensions);
		auto entries = gather(qrcPaths, extensions);
		abcByFileName(entries);
		qDebug() << entries;

		// etc.

		return group;
	}

	inline QActionGroup* make(const QString& qrcPath, QString extension, QWidget* parent = nullptr)
	{
		return make({ qrcPath }, { extension }, {}, parent);
	}
}
