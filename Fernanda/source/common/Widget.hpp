#pragma once

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
};
