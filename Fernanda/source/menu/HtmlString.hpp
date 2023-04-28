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
		template<typename T>
		inline T multiply(T character, int defaultArgument = 1)
		{
			if (defaultArgument < 1)
				defaultArgument = 1;
			if constexpr (std::is_same<T, QString>::value)
				return QString(defaultArgument, character[0]);
			return std::string(defaultArgument, character);
		}

		template<typename T>
		inline T tableColumnSpacing(int columns = 9)
		{
			return multiply("<td>\n</td>", columns);
		}
	}

	template<typename T>
	inline const T table(const std::vector<T>& columns)
	{
		T result = "<table><td>";
		for (auto& column : columns) {
			result += column + "</td>" + tableColumnSpacing<T>();
			result += (column != columns.back())
				? "<td>"
				: "</table>";
		}
		return result;
	}

	template<typename T, typename U = QString>
	inline U bold(const T& text)
	{
		return QString("<b>") + text + QString("</b>");
	}

	/*template<typename U>
	inline U bold(const std::string& text)
	{
		return bold(QString::fromStdString(text));
	}*/

	template<typename T, typename U = QString>
	inline U heading(const T& text, int level = 1)
	{
		if (level < 1 || level > 6)
			level = 1;
		auto level_string = QString::number(level);
		return QString("<h") + level_string + QString("><b>") + text + QString("</b></h") + level_string + QString(">");
	}

	/*template<typename U>
	inline U heading(const std::string& text, int level)
	{
		return heading(QString::fromStdString(text), level);
	}*/

	template<typename T, typename U>
	inline auto link(const T& url, U displayName = U())
	{
		if (displayName.empty())
			displayName = U(url).replace(std::regex("(https:\\/\\/|www.)"), "");
		return T("<a href='") + url + T("'>") + displayName + T("</a>");
	}

	template<typename T>
	inline auto link(const StdFs::path& url, T displayName = T())
	{
		return link(url.generic_string(), displayName);
	}
}
