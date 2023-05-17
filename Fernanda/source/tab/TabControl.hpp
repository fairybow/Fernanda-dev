#pragma once

#include "../common/Layout.hpp"
#include "../common/Utility.hpp"

#include <QTabBar>
#include <QToolButton>

class TabControl : public QWidget
{
	Q_OBJECT

public:
	TabControl(QTabBar* parent = nullptr)
		: QWidget(parent), m_parent(parent)
	{
		connections();
		build();
	}

	void adjust()
	{
		Utility::delayCall(this, [&] {
			if (m_parent->count() < 1) return;
			setScrollButtonsVisible(isFull());
			auto rect = m_parent->tabRect(m_parent->count() - 1);
			auto x = rect.right() + 3;
			auto y = rect.center().y() - (height() / 2) + 1;
			auto max_x = m_parent->width() - width();
			if (x > max_x)
				x = max_x;
			move(x, y);
			});
	}

signals:
	void addTabClicked();

private:
	QTabBar* m_parent;
	QToolButton* m_add = new QToolButton(this);
	QToolButton* m_scrollLeft = new QToolButton(this);
	QToolButton* m_scrollRight = new QToolButton(this);

	void connections()
	{
		connect(m_add, &QToolButton::clicked, this, [&] {
			emit addTabClicked();
			});
		connect(m_scrollLeft, &QToolButton::clicked, this, [&] {
			m_parent->setCurrentIndex(m_parent->currentIndex() - 1);
			});
		connect(m_scrollRight, &QToolButton::clicked, this, [&] {
			m_parent->setCurrentIndex(m_parent->currentIndex() + 1);
			});
	}

	void build()
	{
		m_add->setText("+");
		m_scrollLeft->setText("<");
		m_scrollRight->setText(">");
		auto layout = Layout::box(Layout::Line::Horizontally, { m_add, m_scrollLeft, m_scrollRight }, this);
		Layout::setUniformSpacing(layout, 2);
	}

	void setScrollButtonsVisible(bool visible)
	{
		m_scrollLeft->setVisible(visible);
		m_scrollRight->setVisible(visible);
		adjustSize();
	}

	bool isFull()
	{
		auto tabs_width = 0;
		for (auto i = 0; i < m_parent->count(); ++i)
			tabs_width += m_parent->tabRect(i).width();
		return (tabs_width > m_parent->width());
	}
};
