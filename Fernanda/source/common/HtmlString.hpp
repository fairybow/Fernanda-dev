#pragma once

#include <QRegularExpression>
#include <QString>
#include <QUrl>

#include <string>

inline QString operator%(const QString& lhs, const QString& rhs)
{
	return lhs + "<p>" + rhs;
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
	return lhs + "<br>" + rhs;
}
inline QString operator/(const QString& lhs, const char* rhs) { return lhs / QString(rhs); }
inline QString operator/(const char* lhs, const QString& rhs) { return QString(lhs) / rhs; }
inline QString operator/(const QString& lhs, const std::string& rhs) { return lhs / QString::fromStdString(rhs); }
inline QString operator/(const std::string& lhs, const QString& rhs) { return QString::fromStdString(lhs) / rhs; }

namespace HtmlString
{
	namespace StdFs = std::filesystem;

	inline QString multiply(const char* character, int defaultArgument = 2)
	{
		if (defaultArgument < 1)
			defaultArgument = 1;
		QString multiplied;
		if (character[1] == '\0')
			multiplied = QString(defaultArgument, character[0]);
		else {
			for (int i = 0; i < defaultArgument; ++i)
				multiplied += character;
		}
		return multiplied;
	}

	namespace
	{
		inline QString tableColumnSpacing(int columns = 9)
		{
			return multiply("<td>\n</td>", columns);
		}
	}

	template<typename T>
	inline QString table(const std::vector<T>& columns)
	{
		QString table = "<table><td>";
		for (auto& column : columns) {
			table += column + "</td>" + tableColumnSpacing();
			table += (column != columns.back())
				? "<td>"
				: "</table>";
		}
		return table;
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
		return QString("<h%1>%2</h%1>").arg(level).arg(text);
	}

	inline QString link(const QString& url, QString displayName = QString())
	{
		if (displayName.isEmpty()) {
			QString url_copy = url;
			displayName = url_copy.replace(QRegularExpression("(https:\\/\\/|www.)"), "");
		}
		return QString("<a href='%1'>%2</a>").arg(url).arg(displayName);
	}

	inline QString link(const QUrl& url, QString displayName = QString())
	{
		return link(url.toString(), displayName);
	}

	inline QString link(const char* url, QString displayName = QString())
	{
		return link(QString(url), displayName);
	}
}
