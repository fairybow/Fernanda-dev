#pragma once

#include "../../common/UiButton.hpp"

#include <QPoint>
#include <QTabBar>

class TabCloseButton : public UiButton
{
	Q_OBJECT

public:
	TabCloseButton(QTabBar* tabBar)
		: UiButton(Ui::Close, tabBar, Ui::Ellipse), m_tabBar(tabBar)
	{
		connect(this, &TabCloseButton::clicked, this, [&] {

			auto screen_position = mapToGlobal(QPoint(0, 0));
			auto position_on_bar = m_tabBar->mapFromGlobal(screen_position);
			auto index = m_tabBar->tabAt(position_on_bar);

			emit clickedAt(index);

			});
	}

private:
	QTabBar* m_tabBar;

signals:
	void clickedAt(int index);
};
