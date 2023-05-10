#pragma once

#include <QString>
#include <QTimer>

namespace Utility
{
	template<typename Lambda>
	inline void delayCall(const QObject* context, Lambda lambda)
	{
		QTimer::singleShot(0, context, lambda);
	}

	template<typename... Calls>
	inline void delayCalls(const QObject* context, Calls... call)
	{
		(QTimer::singleShot(0, context, call), ...);
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

	inline int greaterOrEqual(int value, int mustExceed = 1)
	{
		return (value < mustExceed) ? mustExceed : value;
	}

	inline QString padString(const QString& string, int desiredLength, QChar padChar = ' ')
	{
		if (string.length() >= desiredLength)
			return string;
		return string + QString(desiredLength - string.length(), padChar);
	}
}
