#pragma once

#include <QList>
#include <QRandomGenerator>
#include <QRegularExpression>
#include <QString>

#include <ctime>

namespace StringTools
{
	namespace
	{
		constexpr char PANGRAM_1[] = "How vexingly quick daft zebras jump!";
		constexpr char PANGRAM_2[] = "Pack my box with five dozen liquor jugs.";
		constexpr char PANGRAM_3[] = "Sphinx of black quartz, judge my vow.";
		constexpr char PANGRAM_4[] = "The quick brown fox jumps over the lazy dog.";
		constexpr char PANGRAM_5[] = "Waltz, bad nymph, for quick jigs vex.";
		constexpr char REGEX_FORBIDDEN[] = R"((<|>|:|\/|\\|\||\?|\*|\"))";
		constexpr char REGEX_LEADING_WHITESPACE[] = "(\\s|\\n|\\r|\U00002029|^)+";
	}

	inline QString randomLine(QStringList lines)
	{
		auto highest_line = lines.size();
		auto index = QRandomGenerator::global()->bounded(highest_line);

		return lines[index];
	}

	inline QString pangram()
	{
		QStringList pangrams = { PANGRAM_1, PANGRAM_2, PANGRAM_3, PANGRAM_4, PANGRAM_5 };

		return randomLine(pangrams);
	}

	inline QString time()
	{
		auto now = std::time(0);
		auto time = std::ctime(&now);

		return QString::fromLocal8Bit(time).replace("  ", " ").trimmed();
	}

	inline int wordCount(const QString& text)
	{
		auto regex = QRegularExpression(REGEX_LEADING_WHITESPACE);
		auto words = text.split(regex, Qt::SkipEmptyParts);

		return words.count();
	}

	inline QString wordCountString(const QString& text)
	{
		return QString::number(wordCount(text));
	}
}
