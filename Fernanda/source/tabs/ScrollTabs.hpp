#pragma once

#include "../common/Svg.hpp"
#include "TabButton.hpp"

#include <QTabBar>

class ScrollTabs : public TabButton
{
	Q_OBJECT

public:
	enum class Side {
		Left,
		Right
	};

	ScrollTabs(QTabBar* tabBar, Side side, QWidget* parent = nullptr)
		: TabButton("TabButton", icon(side), parent), m_tabBar(tabBar)
	{
		auto delta = (side == Side::Left) ? -1 : 1;
		connect(this, &ScrollTabs::clicked, this, [&] {
			m_tabBar->setCurrentIndex(m_tabBar->currentIndex() + delta);
			});
	}

private:
	QTabBar* m_tabBar;

	Svg::Ui icon(Side side)
	{
		return (side == Side::Left) ? Svg::Ui::ChevronBack : Svg::Ui::ChevronForward;
	}
};
