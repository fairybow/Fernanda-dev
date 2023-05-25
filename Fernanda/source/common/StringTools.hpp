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
		return QString::fromLocal8Bit(time);
	}

	inline QString clean(const QString& string)
	{
		QString cleaned_string = string;
		for (auto& [from, to] : escapeSequences)
			cleaned_string.replace(from, to);
		return cleaned_string;
	}
}
