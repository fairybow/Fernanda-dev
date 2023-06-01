#pragma once

#include <QString>
#include <QVariant>

#include <filesystem>
#include <type_traits>

namespace Path
{
	namespace StdFs = std::filesystem;
	using StdFsPath = StdFs::path;

	namespace
	{
		template <typename T>
		using IsFsPathOrQString = std::disjunction<std::is_same<T, StdFsPath>, std::is_same<T, QString>>;

		inline StdFsPath pathOrParent(StdFsPath path, bool hasFileName)
		{
			return hasFileName ? path.parent_path() : path;
		}
	}

	inline StdFsPath toStdFs(QString qStringPath)
	{
		return StdFsPath(qStringPath.toStdString());
	}

	inline StdFsPath toStdFs(QVariant qVariantPath)
	{
		return toStdFs(qVariantPath.toString());
	}

	inline StdFsPath toStdFs(const char* cStringPath)
	{
		return StdFsPath(cStringPath);
	}

	inline QString toQString(StdFsPath path, bool sanitize = true)
	{
		auto q_path = QString::fromStdString(path.make_preferred().string());
		if (sanitize)
			q_path.replace(R"(\)", R"(/)");
		return q_path;
	}

	inline void make(const StdFsPath& path, bool includesFileName = false)
	{
		auto directory = pathOrParent(path, includesFileName);
		if (!StdFs::exists(directory))
			StdFs::create_directories(directory);
	}

	inline void clear(const StdFsPath& path, bool clearSelf = false, bool includesFileName = false)
	{
		auto directory = pathOrParent(path, includesFileName);
		if (!StdFs::exists(directory)) return;
		for (auto& item : StdFs::directory_iterator(directory))
			StdFs::remove_all(item);
		if (clearSelf)
			StdFs::remove(directory);
	}

	inline StdFsPath name(const StdFsPath& path, bool keepExtension = false)
	{
		return keepExtension ? path.filename() : path.stem();
	}

	inline StdFsPath name(const QString& path, bool keepExtension = false)
	{
		return name(toStdFs(path), keepExtension);
	}

	inline StdFsPath parentName(const StdFsPath& path, bool keepExtension = false)
	{
		return name(path.parent_path(), keepExtension);
	}

	inline StdFsPath parentName(const QString& path, bool keepExtension = false)
	{
		return name(toStdFs(path).parent_path(), keepExtension);
	}

	template<typename T>
	inline typename std::enable_if<IsFsPathOrQString<T>::value, QString>::type
		qStringName(const T& path, bool keepExtension = false)
	{
		return QString::fromStdString(name(path, keepExtension).string());
	}

	inline QString qStringParentName(const StdFsPath& path, bool keepExtension = false)
	{
		return qStringName(path.parent_path(), keepExtension);
	}

	inline QString qStringParentName(const QString& path, bool keepExtension = false)
	{
		return qStringName(toStdFs(path).parent_path(), keepExtension);
	}
}
