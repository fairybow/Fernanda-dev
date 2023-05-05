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

	static inline void transpareForMouse(QWidgetList widgets)
	{
		for (auto& widget : widgets)
			widget->setAttribute(Qt::WA_TransparentForMouseEvents);
	}

	static inline void transpareForMouse(QWidget* widget) { transpareForMouse({ widget }); }
};
