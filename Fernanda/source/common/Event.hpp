#pragma once

#include <QTimer>

namespace Event
{
	template<typename... Signals>
	inline void delay(const QObject* context, Signals... signal)
	{
		(QTimer::singleShot(0, context, signal), ...);
	}
}
