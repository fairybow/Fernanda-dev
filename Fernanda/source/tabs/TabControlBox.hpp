#pragma once

#include "../common/Layout.hpp"
#include "../common/Svg.hpp"
#include "AddTab.hpp"
#include "ScrollTabs.hpp"

class TabControlBox : public QWidget
{
	Q_OBJECT

public:
	TabControlBox(QTabBar* tabBar, QWidget* parent = nullptr)
		: QWidget(parent),
		m_scrollLeft(new ScrollTabs(tabBar, ScrollTabs::Side::Left)),
		m_scrollRight(new ScrollTabs(tabBar, ScrollTabs::Side::Right))
	{
		build();
	}

	void setScrollerVisible(bool visible)
	{
		m_scrollLeft->setVisible(visible);
		m_scrollRight->setVisible(visible);
		layout()->update();
	}

signals:
	void addTabClicked();

private:
	AddTab* m_add = new AddTab;
	ScrollTabs* m_scrollLeft;
	ScrollTabs* m_scrollRight;

	void build()
	{
		connect(m_add, &AddTab::addTabClicked, this, [&] {
			emit addTabClicked();
			});
		auto layout = Layout::box(Layout::Line::Horizontally, { m_add, m_scrollLeft, m_scrollRight }, this);
		layout->setContentsMargins(0, 0, 4, 0);
	}
};
