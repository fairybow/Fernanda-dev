#pragma once

#include <QHoverEvent>
#include <QSize>
#include <QStyle>
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
	virtual bool event(QEvent* event) override
	{
		if (event->type() == QEvent::HoverEnter || event->type() == QEvent::HoverLeave) {
			qDebug() << "Hover event:" << (event->type() == QEvent::HoverEnter ? "Enter" : "Leave");
			auto hover_event = static_cast<QHoverEvent*>(event);
			auto index = tabAt(hover_event->pos());
			if (index != -1) {
				auto button = tabButton(index, QTabBar::RightSide);
				if (button) {
					button->setProperty("tab-hover", (event->type() == QEvent::HoverEnter));
					button->style()->unpolish(button);
					button->style()->polish(button);

					qDebug() << "Tab-hover property:" << button->property("tab-hover");
				}
			}
		}
		return QTabBar::event(event);
	}

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
	const std::pair<int, int> m_tabSizeRange;
};
