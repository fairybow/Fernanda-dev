#pragma once

#include "../common/Widget.hpp"

#include <QTabBar>

class TabBar : public Widget<QTabBar>
{
public:
	using Widget::Widget;
};
