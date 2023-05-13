#pragma once

#include "common/Widget.hpp"

#include <QTabBar>

class TabBar : public Widget<QTabBar>
{
public:
	TabBar(const char* name, QWidget* parent = nullptr)
		: Widget(name, parent)
	{
		setShape(Shape::RoundedNorth);
	}
};
