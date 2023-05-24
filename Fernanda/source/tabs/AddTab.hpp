#pragma once

#include "../common/Icon.hpp"
#include "TabButton.hpp"

class AddTab : public TabButton
{
public:
	AddTab(QWidget* parent = nullptr)
		: TabButton("TabButton", Icon::Ui::Add, parent) {}
};
