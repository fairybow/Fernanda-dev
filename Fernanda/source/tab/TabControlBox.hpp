#pragma once

#include "../common/Layout.hpp"

#include <QTabBar>
#include <QToolButton>

class TabControlBox : public QWidget
{
	Q_OBJECT

public:
	TabControlBox(QTabBar* tabBar, QWidget* parent = nullptr)
		: QWidget(parent), m_tabBar(tabBar)
	{
		build();
		connections();
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
	QTabBar* m_tabBar;
	QToolButton* m_add = new QToolButton;
	QToolButton* m_scrollLeft = new QToolButton;
	QToolButton* m_scrollRight = new QToolButton;

	void build()
	{
		m_add->setText("+");
		m_scrollLeft->setText("<");
		m_scrollRight->setText(">");
		Layout::box(Layout::Line::Horizontally, { m_add, m_scrollLeft, m_scrollRight }, this);
	}

	void connections()
	{
		connect(m_add, &QToolButton::clicked, this, [&] {
			emit addTabClicked();
			});
		connect(m_scrollLeft, &QToolButton::clicked, this, [&] {
			m_tabBar->setCurrentIndex(m_tabBar->currentIndex() - 1);
			});
		connect(m_scrollRight, &QToolButton::clicked, this, [&] {
			m_tabBar->setCurrentIndex(m_tabBar->currentIndex() + 1);
			});
	}
};
