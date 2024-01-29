#pragma once

//#include <QDataStream>
#include <QDebug>
#include <QStandardPaths>
#include <QString>
#include <QVariant>

#include <filesystem>
#include <ostream>
#include <string>
#include <unordered_map>

class Path
{
public:
	Path() : m_path(std::filesystem::path()) {}
	Path(const std::filesystem::path& path) : m_path(path) {}
	Path(const char* cStringPath) : m_path(cStringPath) {}
	Path(const std::string& stringPath) : m_path(stringPath) {}
	Path(const QString& qStringPath) : m_path(qStringPath.toStdString()) {}
	Path(const QVariant& qVariantPath) : m_path(qVariantPath.toString().toStdString()) {}

	enum class System {
		AppConfig,
		AppData,
		AppLocalData,
		Applications,
		Cache,
		Config,
		Desktop,
		Download,
		Documents,
		Fonts,
		GenericCache,
		GenericConfig,
		GenericData,
		Home,
		Movies,
		Music,
		Pictures,
		PublicShare,
		Runtime,
		Temp,
		Templates,
		END
	};

	operator std::filesystem::path() const
	{
		return m_path;
	}

	operator QVariant() const
	{
		return toQVariant();
	}

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
		return !(*this == other);
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

	/*friend QDataStream& operator<<(QDataStream& out, const Path& path)
	{
		out << path.toQString();

		return out;
	}

	friend QDataStream& operator>>(QDataStream& in, Path& path)
	{
		QString serialized;
		in >> serialized;
		path = Path(serialized);

		return in;
	}*/

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

	static Path system(System type)
	{
		auto qt_types = qtTypes();
		auto it = qt_types.find(type);

		if (it != qt_types.end())
			return Path(QStandardPaths::locate(it->second, QString(), QStandardPaths::LocateDirectory));

		return Path();
	}

	static QList<Path> systemDirs()
	{
		QList<Path> paths;

		for (auto i = 0; i < static_cast<int>(System::END); ++i)
			paths << system(static_cast<System>(i));

		return paths;
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
		//return QVariant::fromValue(*this);
		return QVariant::fromValue(toQString());
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

	static const std::unordered_map<System, QStandardPaths::StandardLocation> qtTypes()
	{
		static const std::unordered_map<System, QStandardPaths::StandardLocation> qt_types = {
			{ System::AppConfig, QStandardPaths::AppConfigLocation },
			{ System::AppData, QStandardPaths::AppDataLocation },
			{ System::AppLocalData, QStandardPaths::AppLocalDataLocation },
			{ System::Applications, QStandardPaths::ApplicationsLocation },
			{ System::Cache, QStandardPaths::CacheLocation },
			{ System::Config, QStandardPaths::ConfigLocation },
			{ System::Desktop, QStandardPaths::DesktopLocation },
			{ System::Download, QStandardPaths::DownloadLocation },
			{ System::Documents, QStandardPaths::DocumentsLocation },
			{ System::Fonts, QStandardPaths::FontsLocation },
			{ System::GenericCache, QStandardPaths::GenericCacheLocation },
			{ System::GenericConfig, QStandardPaths::GenericConfigLocation },
			{ System::GenericData, QStandardPaths::GenericDataLocation },
			{ System::Home, QStandardPaths::HomeLocation },
			{ System::Movies, QStandardPaths::MoviesLocation },
			{ System::Music, QStandardPaths::MusicLocation },
			{ System::Pictures, QStandardPaths::PicturesLocation },
			{ System::PublicShare, QStandardPaths::PublicShareLocation },
			{ System::Runtime, QStandardPaths::RuntimeLocation },
			{ System::Temp, QStandardPaths::TempLocation },
			{ System::Templates, QStandardPaths::TemplatesLocation }
		};

		return qt_types;
	}
};

template<>
static inline QVariant QVariant::fromValue<Path>(const Path& value)
{
	return QVariant::fromValue(value.toQString());
}

template<>
static inline QVariant QVariant::fromValue<Path>(Path&& value)
{
	return QVariant::fromValue(value.toQString());
}

template<>
inline Path QVariant::value<Path>() const
{
	if (canConvert<QString>())
		return Path(toString());

	return Path();
}
