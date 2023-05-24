#pragma once

#include "../common/Icon.hpp"
#include "TabButton.hpp"

#include <QTabBar>

class ScrollTabs : public TabButton
{
	Q_OBJECT

public:
	enum class Side { Left, Right };

	ScrollTabs(QTabBar* tabBar, Side side, QWidget* parent = nullptr)
		: TabButton("TabButton", icon(side), parent), m_tabBar(tabBar), m_side(side)
	{
		connect(this, &ScrollTabs::clicked, this, [&] {
			auto delta = (m_side == Side::Left) ? -1 : 1;
			m_tabBar->setCurrentIndex(m_tabBar->currentIndex() + delta);
			});

		// scroll wheel either direction could work on either button?
	}

private:
	QTabBar* m_tabBar;
	Side m_side;

	Icon::Ui icon(Side side)
	{
		return (side == Side::Left) ? Icon::Ui::ChevronLeft : Icon::Ui::ChevronRight;
	}
};
