#pragma once

#include <QString>
#include <QVector>

#include <ctime>
#include <utility>

namespace StringTools
{
	using QStringPair = std::pair<QString, QString>;

	namespace
	{
		const QVector<QStringPair> escapeSequences = {
		{"\\\\", "\\"},
		{"\\'", "\'"},
		{"\\\"", "\""},
		{"\\?", "\?"},
		{"\\a", "\a"},
		{"\\b", "\b"},
		{"\\f", "\f"},
		{"\\n", "\n"},
		{"\\r", "\r"},
		{"\\t", "\t"},
		{"\\v", "\v"}
		};
	}

	inline QString time()
	{
		auto now = std::time(0);
		auto time = std::ctime(&now);
		return QString::fromLocal8Bit(time).trimmed();
	}

	inline QString clean(const QString& string)
	{
		QString cleaned_string = string;
		for (auto& [from, to] : escapeSequences)
			cleaned_string.replace(from, to);
		return cleaned_string;
	}

	inline const QString secondsToMinutes(int seconds, const char* separator = ":")
	{
		auto time_seconds = seconds % 60;
		QString seconds_string;
		(time_seconds <= 9)
			? seconds_string = "0" + QString::number(time_seconds)
			: seconds_string = QString::number(time_seconds);
		return QString::number((seconds / 60) % 60) + separator + seconds_string;
	}

	inline QString sophisticatedPluralCheckThatAlwaysWorks(const QString& text, int value)
	{
		if (!text.endsWith("s") || value != 1) return text;
		return text.left(text.length() - 1);
	}

	inline QString pad(const QString& string, int desiredLength, QChar padChar = ' ')
	{
		if (string.length() >= desiredLength)
			return string;
		return string + QString(desiredLength - string.length(), padChar);
	}
}
