#pragma once

#include "../common/Icon.hpp"
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
		m_add->setIcon(Icon::ui(Icon::Ui::Add));
		m_scrollLeft->setIcon(Icon::ui(Icon::Ui::ChevronBack));
		m_scrollRight->setIcon(Icon::ui(Icon::Ui::ChevronForward));
		auto layout = Layout::box(Layout::Line::Horizontally, { m_add, m_scrollLeft, m_scrollRight }, this);
		layout->setContentsMargins(0, 0, 4, 0);
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
