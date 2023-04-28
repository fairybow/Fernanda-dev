#pragma once

#include <QRegularExpression>
#include <QUrl>

#include <filesystem>
#include <string>
#include <type_traits>

inline QString operator%(const QString& lhs, const QString& rhs)
{
	return lhs + QStringLiteral("<p>") + rhs;
}
inline QString operator%(const QString& lhs, const char* rhs) { return lhs % QString(rhs); }
inline QString operator%(const char* lhs, const QString& rhs) { return QString(lhs) % rhs; }
inline QString operator%(const QString& lhs, const std::string& rhs) { return lhs % QString::fromStdString(rhs); }
inline QString operator%(const std::string& lhs, const QString& rhs) { return QString::fromStdString(lhs) % rhs; }

inline QString operator%=(QString& lhs, const QString& rhs)
{
	lhs = lhs % rhs;
	return lhs;
}

inline QString operator/(const QString& lhs, const QString& rhs)
{
	return lhs + QStringLiteral("<br>") + rhs;
}
inline QString operator/(const QString& lhs, const char* rhs) { return lhs / QString(rhs); }
inline QString operator/(const char* lhs, const QString& rhs) { return QString(lhs) / rhs; }
inline QString operator/(const QString& lhs, const std::string& rhs) { return lhs / QString::fromStdString(rhs); }
inline QString operator/(const std::string& lhs, const QString& rhs) { return QString::fromStdString(lhs) / rhs; }

namespace HtmlString
{
	namespace StdFs = std::filesystem;

	inline QString multiply(const char* character, int defaultArgument = 1)
	{
		if (defaultArgument < 1)
			defaultArgument = 1;
		QString result;
		if (character[1] == '\0')
			result = QString(defaultArgument, character[0]);
		else {
			for (int i = 0; i < defaultArgument; ++i)
				result += character;
		}
		return result;
	}

	namespace {
		inline QString tableColumnSpacing(int columns = 9)
		{
			return multiply("<td>\n</td>", columns);
		}
	}

	template<typename T>
	inline QString table(const std::vector<T>& columns)
	{
		QString result = "<table><td>";
		for (auto& column : columns) {
			result += column + "</td>" + tableColumnSpacing();
			result += (column != columns.back())
				? "<td>"
				: "</table>";
		}
		return result;
	}

	template<typename T>
	inline QString bold(const T& text)
	{
		return QString("<b>%1</b>").arg(text);
	}

	template<typename T>
	inline QString heading(const T& text, int level = 1)
	{
		level = qBound(1, level, 6);
		return QString("<h%1><b>%2</b></h%1>").arg(level).arg(text);
	}

	inline QString link(QString url, QString displayName = QString())
	{
		if (displayName.isEmpty())
			displayName = url.replace(QRegularExpression("(https:\\/\\/|www.)"), "");
		return QString("<a href='%1'>%2</a>").arg(url).arg(displayName);
	}

	inline QString link(const QUrl& url, QString displayName = QString())
	{
		return link(url.toString(), displayName);
	}

	inline QString link(const StdFs::path& url, QString displayName = QString())
	{
		return link(QString::fromStdString(url.generic_string()), displayName); // Path::
	}
}
