#pragma once

#include <QString>

#include <filesystem>
#include <regex>
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

	namespace {
		inline QString multiply(const QString& character, int defaultArgument = 1)
		{
			if (defaultArgument < 1)
				defaultArgument = 1;
			QString result;
			for (int i = 0; i < defaultArgument; ++i)
				result += character;
			return result;
		}

		inline QString multiply(const char* character, int defaultArgument = 1)
		{
			return multiply(QString(character), defaultArgument);
		}

		inline QString multiply(QChar character, int defaultArgument = 1)
		{
			return QString(defaultArgument, character);
		}

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

	template<typename T, typename U>
	inline QString link(const T& url, U displayName = U())
	{
		if (displayName.empty())
			displayName = U(url).replace(std::regex("(https:\\/\\/|www.)"), "");
		return QString("<a href='%1'>%2</a>").arg(url).arg(displayName);
	}

	template<typename T>
	inline QString link(const StdFs::path& url, T displayName = T())
	{
		return link(url.generic_string(), displayName);
	}
}
