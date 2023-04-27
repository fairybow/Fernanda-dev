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

	template<typename T>
	inline T bold(const T& text)
	{
		return T("<b>") + text + T("</b>");
	}

	template<typename T>
	inline T heading(const T& text, int level = 1)
	{
		if (level < 1 || level > 6)
			level = 1;
		auto level_string = std::to_string(level);
		return T("<h") + level_string + T("><b>") + text + T("</b></h") + level_string + T(">");
	}

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
