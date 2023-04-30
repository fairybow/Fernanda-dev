#pragma once

#include <QDir>
#include <QString>
#include <QVariant>

#include <filesystem>

namespace Path
{
	namespace StdFs = std::filesystem;

	inline StdFs::path toStdFs(QString qStringPath)
	{
		return StdFs::path(qStringPath.toStdString());
	}

	inline StdFs::path toStdFs(QVariant qVariantPath)
	{
		return toStdFs(qVariantPath.toString());
	}

	inline StdFs::path toStdFs(const char* cStringPath)
	{
		return StdFs::path(cStringPath);
	}

	inline QString toQString(StdFs::path path, bool sanitize = false)
	{
		auto qpath = QString::fromStdString(path.make_preferred().string());
		if (sanitize)
			qpath.replace(R"(\)", R"(/)");
		return qpath;
	}

	namespace
	{
		inline StdFs::path pathOrParentBasedOnFileName(StdFs::path path, bool includes)
		{
			return includes ? path.parent_path() : path;
		}
	}

	inline void make(StdFs::path path, bool includesFileName = false)
	{
		auto directory = pathOrParentBasedOnFileName(path, includesFileName);
		if (!QDir(directory).exists())
			StdFs::create_directories(directory);
	}

	inline void clear(StdFs::path path, bool clearSelf = false, bool includesFileName = false)
	{
		auto directory = pathOrParentBasedOnFileName(path, includesFileName);
		if (!StdFs::exists(directory)) return;
		for (auto& item : StdFs::directory_iterator(directory))
			StdFs::remove_all(item);
		if (clearSelf)
			StdFs::remove(directory);
	}
}
