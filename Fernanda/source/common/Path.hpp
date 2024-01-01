#pragma once

#include <QDebug>
#include <QString>
#include <QVariant>

#include <filesystem>
#include <ostream>
#include <string>

class Path
{
public:
	Path() : m_path(std::filesystem::path()) {}
	Path(const std::filesystem::path& path) : m_path(path) {}
	Path(const char* cStringPath) : m_path(cStringPath) {}
	Path(const std::string& stringPath) : m_path(stringPath) {}
	Path(const QString& qStringPath) : m_path(qStringPath.toStdString()) {}
	Path(const QVariant& qVariantPath) : m_path(qVariantPath.toString().toStdString()) {}

	explicit operator bool() const
	{
		return !m_path.empty();
	}

	bool operator==(const Path& other) const
	{
		return m_path == other.m_path;
	}

	bool operator!=(const Path& other) const
	{
		return m_path != other.m_path;
	}

	Path& operator=(const Path& other)
	{
		if (this != &other)
			m_path = other.m_path;

		return *this;
	}

	Path& operator/=(const Path& other)
	{
		m_path /= other.m_path;

		return *this;
	}

	Path operator/(const Path& other) const
	{
		Path result = *this;
		result /= other;

		return result;
	}

	friend std::ostream& operator<<(std::ostream& os, const Path& path)
	{
		os << path.toString();

		return os;
	}

	friend QDebug operator<<(QDebug debug, const Path& path)
	{
		debug.nospace() << path.toQString();

		return debug.maybeSpace();
	}

	bool isFile() const
	{
		return std::filesystem::is_regular_file(m_path);
	}

	bool isFolder() const
	{
		return std::filesystem::is_directory(m_path);
	}

	bool isValid() const
	{
		return std::filesystem::exists(m_path);
	}

	static bool createDirs(const Path& path)
	{
		return std::filesystem::create_directories(path.toStd());
	}

	std::filesystem::path toStd() const
	{
		return m_path;
	}

	std::string toString() const
	{
		return m_path.string();
	}

	QString toQString() const
	{
		return QString::fromStdString(toString());
	}

	QVariant toQVariant() const
	{
		return QVariant(toQString());
	}

	Path stem() const
	{
		return m_path.stem();
	}

	std::string name() const
	{
		return stem().toString();
	}

	QString qStringName() const
	{
		return QString::fromStdString(name());
	}

	Path file() const
	{
		return m_path.filename();
	}

	std::string fileString() const
	{
		return file().toString();
	}

	QString fileQString() const
	{
		return QString::fromStdString(fileString());
	}

	Path ext() const
	{
		return m_path.extension();
	}

	std::string extString() const
	{
		return ext().toString();
	}

	QString extQString() const
	{
		return QString::fromStdString(extString());
	}

	Path parent() const
	{
		return m_path.parent_path();
	}

	std::string parentString() const
	{
		return parent().name();
	}

	QString parentQString() const
	{
		return QString::fromStdString(parentString());
	}

private:
	std::filesystem::path m_path;
};
