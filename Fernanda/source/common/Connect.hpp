#pragma once

#include <QObject>

#define sender_cast(TSenderType) qobject_cast<TSenderType*>(QObject::sender())

namespace Connect
{
	template <typename TSender, typename TReceiver, typename TSlot, typename... TSignals>
	void multi(TSender sender, TReceiver receiver, TSlot slot, TSignals... signal)
	{
		(QObject::connect(sender, signal, receiver, slot), ...);
	}
}
