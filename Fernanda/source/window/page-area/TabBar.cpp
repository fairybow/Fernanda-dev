#include "TabBar.h"

TabBar::TabBar(QWidget* parent)
	: QTabBar(parent)
{}

bool TabBar::isFull() const
{
	return tabsWidth() > width();
}

bool TabBar::isEmpty() const
{
	return count() < 1;
}

void TabBar::resizeEvent(QResizeEvent* event)
{
	QTabBar::resizeEvent(event);

	emit resized();
}

void TabBar::tabInserted(int index)
{
	QTabBar::tabInserted(index);

	emit tabCountChanged();
}

void TabBar::tabRemoved(int index)
{
	QTabBar::tabRemoved(index);

	emit tabCountChanged();
}

int TabBar::tabsWidth() const
{
	auto tabs_width = 0;

	for (auto i = 0; i < count(); ++i)
		tabs_width += tabRect(i).width();

	return tabs_width;
}
