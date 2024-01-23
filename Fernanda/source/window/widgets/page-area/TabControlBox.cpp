#include "../../../common/Layout.hpp"
#include "TabControlBox.h"

#include <QList>
#include <QMargins>

TabControlBox::TabControlBox(QTabBar* tabBar, QWidget* parent)
	: QWidget(parent), m_tabBar(tabBar)
{
	setup();
}

void TabControlBox::setShowScrolls(bool visible)
{
	m_scrollLeft->setVisible(visible);
	m_scrollRight->setVisible(visible);

	layout()->update();
}

void TabControlBox::wheelEvent(QWheelEvent* event)
{
	(event->angleDelta().y() > 0)
		? onScrollLeftClicked()
		: onScrollRightClicked();
}

void TabControlBox::setup()
{
	sizing();
	connections();

	m_addTab->setAutoRepeat(false);
	m_scrollLeft->setAutoRepeat(true);
	m_scrollRight->setAutoRepeat(true);
}

void TabControlBox::sizing()
{
	QObjectList buttons{ m_addTab, m_scrollLeft, m_scrollRight };
	QMargins margins(4, 0, 4, 0);
	Layout::box(Layout::Box::Horizontal, this, buttons, margins);

	setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	setFixedHeight(m_tabBar->height());
}

void TabControlBox::connections()
{
	connect(m_addTab, &UiButton::clicked, this, [&] { emit addTabClicked(); });
	connect(m_scrollLeft, &UiButton::clicked, this, &TabControlBox::onScrollLeftClicked);
	connect(m_scrollRight, &UiButton::clicked, this, &TabControlBox::onScrollRightClicked);
}

void TabControlBox::onScrollLeftClicked()
{
	m_tabBar->setCurrentIndex(m_tabBar->currentIndex() - 1);
}

void TabControlBox::onScrollRightClicked()
{
	m_tabBar->setCurrentIndex(m_tabBar->currentIndex() + 1);
}
