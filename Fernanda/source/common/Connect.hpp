#pragma once

#include <QObject>

#define sender_cast(TSenderType) qobject_cast<TSenderType*>(QObject::sender())

namespace Connect
{
	template <typename Sender, typename Receiver, typename Slot, typename... Signals>
	void multi(Sender sender, Receiver receiver, Slot slot, Signals... signal)
	{
		(QObject::connect(sender, signal, receiver, slot), ...);
	}
}
