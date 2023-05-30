#pragma once

#include "../common/UiButton.hpp"

#include <QTabBar>

class ScrollTabs : public UiButton
{
	Q_OBJECT

public:
	enum class Side { Left, Right };

	ScrollTabs(QTabBar* tabBar, Side side, QWidget* parent = nullptr)
		: UiButton("TabButton", icon(side), parent), m_tabBar(tabBar), m_side(side)
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

	Ui icon(Side side)
	{
		return (side == Side::Left) ? Ui::ChevronLeft : Ui::ChevronRight;
	}
};
