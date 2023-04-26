#pragma once

#include <QWidget>

class Meter : public QWidget
{
public:
	inline Meter(const char* name, QWidget* parent)
		: QWidget(parent)
	{
		setObjectName(name);
	}
};
