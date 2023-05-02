#pragma once

#include "../common/Path.hpp"

#include <QActionGroup>
#include <QDirIterator>
#include <QStringList>
#include <QVector>

#include <algorithm>
#include <functional>

namespace MenuGroups
{
	struct Bespoke {
		const QVariant data;
		const QString label;
	};

	using BespokeList = QVector<Bespoke>;
	using StdFsPath = std::filesystem::path;
	using StdFsPathList = QVector<StdFsPath>;

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
		StdFsPathList systemPaths = {}, QWidget* parent = nullptr, std::function<void()> slot = nullptr)
	{
		auto group = new QActionGroup(parent);
		checkExtensions(extensions);
		auto entries = gather(qrcPaths, extensions);
		abcByFileName(entries);

		// etc.

		return group;
	}

	inline QActionGroup* make(const QString& qrcPath, QString extension,
		StdFsPath systemPath = StdFsPath(), QWidget* parent = nullptr, std::function<void()> slot = nullptr)
	{
		return make(QStringList{ qrcPath }, QStringList{ extension },
			StdFsPathList{ systemPath }, parent, slot);
	}

	inline QActionGroup* make(const QStringList& qrcPaths, QStringList extensions,
		StdFsPath systemPath = StdFsPath(), QWidget* parent = nullptr, std::function<void()> slot = nullptr)
	{
		return make(qrcPaths, extensions,
			StdFsPathList{ systemPath }, parent, slot);
	}

	inline QActionGroup* makeBespoke(BespokeList entries, QWidget* parent = nullptr)
	{
		auto group = new QActionGroup(parent);

		// etc.

		return group;
	}


}
