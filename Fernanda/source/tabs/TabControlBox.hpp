#pragma once

#include "../common/Layout.hpp"
#include "../common/Svg.hpp"

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
		m_add->setIcon(Svg::ui(Svg::Ui::Add, Qt::red));
		m_scrollLeft->setIcon(Svg::ui(Svg::Ui::ChevronBack, Qt::red));
		m_scrollRight->setIcon(Svg::ui(Svg::Ui::ChevronForward, Qt::red));
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
