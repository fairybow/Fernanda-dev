#pragma once

#include <QSize>
#include <QTabBar>

#include <utility>

class TrueTabBar : public QTabBar
{
	Q_OBJECT

public:
	TrueTabBar(int minTabSize, int maxTabSize, QWidget* parent = nullptr)
		: QTabBar(parent), m_tabSizeRange(minTabSize, maxTabSize)
	{
		setAutoHide(false);
		setMovable(true);
		setExpanding(false);
		setUsesScrollButtons(true);
		setDrawBase(false);
	}

signals:
	void resized();
	void inserted();
	void removed();

protected:
	virtual void resizeEvent(QResizeEvent* event) override
	{
		QTabBar::resizeEvent(event);
		emit resized();
	}

	virtual void tabInserted(int index) override
	{
		QTabBar::tabInserted(index);
		emit inserted();
	}

	virtual void tabRemoved(int index) override
	{
		QTabBar::tabRemoved(index);
		emit removed();
	}

	virtual QSize tabSizeHint(int index) const override
	{
		auto tab_width = qMin(qMax(width() / count(), m_tabSizeRange.first), m_tabSizeRange.second);
		return QSize(tab_width, QTabBar::tabSizeHint(index).height());
	}

private:
	std::pair<int, int> m_tabSizeRange;
};
