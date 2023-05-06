#pragma once

#include <QVector>
#include <QWidget>

#include <utility>

template<typename T = QWidget, typename... Args>
class Widget : public T
{
public:
	explicit Widget(const char* objectName, QWidget* parent = nullptr, Args&&... args)
		: T(parent, std::forward<Args>(args)...)
	{
		this->setObjectName(objectName);
	}

	inline void transpareForMouse(QWidgetList widgets)
	{
		for (auto& widget : widgets)
			widget->setAttribute(Qt::WA_TransparentForMouseEvents);
	}

	inline void transpareForMouse(QWidget* widget) { transpareForMouse({ widget }); }

	template<typename Sender, typename Receiver, typename Slot, typename... Signals>
	inline void connectMultiple(Sender sender, Receiver receiver, Slot slot, Signals... signal)
	{
		(QObject::connect(sender, signal, receiver, slot), ...);
	}
};
