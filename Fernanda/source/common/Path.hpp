#pragma once

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
}
