#pragma once

#include "../common/Svg.hpp"
#include "TabButton.hpp"

class AddTab : public TabButton
{
	Q_OBJECT

public:
	AddTab(QWidget* parent = nullptr)
		: TabButton("TabButton", Svg::Ui::Add, parent)
	{
		connect(this, &TabButton::clicked, this, [&] {
			emit addTabClicked();
			});
	}

signals:
	void addTabClicked();
};
