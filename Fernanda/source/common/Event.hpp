#pragma once

#include <QTimer>

namespace Event
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
}
