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

	inline void makeDirectories(StdFs::path path, bool pathIncludesFile = false)
	{
		auto directory = pathIncludesFile ? path.parent_path() : path;
		if (!QDir(directory).exists())
			StdFs::create_directories(directory);
	}
}
