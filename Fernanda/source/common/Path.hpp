#pragma once

#include <QString>
#include <QVariant>

#include <filesystem>

namespace Path
{
	namespace StdFs = std::filesystem;

	namespace
	{
		inline StdFs::path pathOrParent(StdFs::path path, bool hasFileName)
		{
			return hasFileName ? path.parent_path() : path;
		}
	}

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

	inline QString toQString(StdFs::path path, bool sanitize = true)
	{
		auto q_path = QString::fromStdString(path.make_preferred().string());
		if (sanitize)
			q_path.replace(R"(\)", R"(/)");
		return q_path;
	}

	inline void make(StdFs::path path, bool includesFileName = false)
	{
		auto directory = pathOrParent(path, includesFileName);
		if (!StdFs::exists(directory))
			StdFs::create_directories(directory);
	}

	inline void clear(StdFs::path path, bool clearSelf = false, bool includesFileName = false)
	{
		auto directory = pathOrParent(path, includesFileName);
		if (!StdFs::exists(directory)) return;
		for (auto& item : StdFs::directory_iterator(directory))
			StdFs::remove_all(item);
		if (clearSelf)
			StdFs::remove(directory);
	}
}
