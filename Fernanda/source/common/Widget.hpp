#pragma once

#include <QWidget>

#include <utility>

template<typename T = QWidget, typename... Args>
class Widget : public T
{
public:
	explicit Widget(QWidget* parent = nullptr, Args&&... args)
		: T(parent, std::forward<Args>(args)...) {}

	explicit Widget(const char* objectName, QWidget* parent = nullptr, Args&&... args)
		: T(parent, std::forward<Args>(args)...)
	{
		this->setObjectName(objectName);
	}

	template<typename Sender, typename Receiver, typename Slot, typename... Signals>
	inline void connectMultiple(Sender sender, Receiver receiver, Slot slot, Signals... signal)
	{
		(QObject::connect(sender, signal, receiver, slot), ...);
	}
};
