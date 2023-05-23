#pragma once

#include "../common/Svg.hpp"
#include "TabButton.hpp"

class AddTab : public TabButton
{
public:
	AddTab(QWidget* parent = nullptr)
		: TabButton("TabButton", Svg::Ui::Add, parent) {}
};
