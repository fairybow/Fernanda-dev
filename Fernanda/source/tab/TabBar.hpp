#pragma once

#include "../common/Layout.hpp"
#include "../common/Widget.hpp"
#include "TabControl.hpp"
#include "TrueTabBar.hpp"

class TabBar : public Widget<>
{
	Q_OBJECT

public:
	TabBar(const char* name, QWidget* parent = nullptr)
		: Widget(name, parent)
	{
		connect(m_trueTabBar, &TrueTabBar::currentChanged, this, [&](int index) {
			emit currentChanged(index);
			});
	}

	//a container that acts as a tab bar and it has a plus button like we have in tab

	//then a controller container that is hidden but has all 3 buttons

	//when tab isFull, hide tab bar's plus button and activate controller in the HBox layout

	// TrueTabBar intermediaries
	//

signals:
	void currentChanged(int index);
	void askNew();

private:
	TrueTabBar* m_trueTabBar = new TrueTabBar;
};
