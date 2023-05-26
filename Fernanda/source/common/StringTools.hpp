#pragma once

#include <QString>
#include <QVector>

#include <ctime>
#include <utility>

namespace StringTools
{
	using QStringPair = std::pair<QString, QString>;

	enum class Side { Both, Left, Right };

	namespace
	{
		const QVector<QStringPair> doubleEscapeReplacements = {
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

		bool isEven(int x)
		{
			return x % 2 == 0;
		}
	}

	inline QString time()
	{
		auto now = std::time(0);
		auto time = std::ctime(&now);
		return QString::fromLocal8Bit(time).trimmed();
	}

	inline QString fixEscapes(const QString& string)
	{
		QString cleaned_string = string;
		for (auto& [from, to] : doubleEscapeReplacements)
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

	inline QString pad(const QString& string, int desiredLength, Side side = Side::Right,
		QChar padChar = ' ', bool separate = false)
	{
		auto length = string.length();
		if (length >= desiredLength)
			return string;

		auto times = desiredLength - length;
		if (side == Side::Both) {
			if (!isEven(times))
				--times;
			times /= 2;
		}

		auto padding = QString(padChar).repeated(times);

		QString separator;
		if (separate) {
			separator = ' ';
			padding = padding.mid(0, padding.length() - 1);
		}

		QString padded;
		switch (side) {
		case Side::Both:
			padded = padding + separator + string + separator + padding;
			break;
		case Side::Left:
			padded = padding + separator + string;
			break;
		case Side::Right:
			padded = string + separator + padding;
			break;
		}
		return padded;
	}
}
