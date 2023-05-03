#pragma once

#include "common/Widget.hpp"

#include <QStatusBar>

class StatusBar : public Widget<QStatusBar>
{
public:
	inline StatusBar(const char* name, QWidget* parent = nullptr)
		: Widget(name, parent)
	{
		setMaximumHeight(22);
	}
};
