#pragma once

#include <QMouseEvent>
#include <QTabBar>

class TrueTabBar : public QTabBar
{
	Q_OBJECT

public:
	TrueTabBar(QWidget* parent = nullptr)
		: QTabBar(parent)
	{
		setAutoHide(false);
		setTabsClosable(true);
		setMovable(true);
		setExpanding(false);
		setUsesScrollButtons(true);
	}

signals:
	void mousePressed();
	void mouseMoved();
	void mouseReleased();
	void resized();
	void inserted();
	void removed();

protected:
	virtual void mousePressEvent(QMouseEvent* event) override
	{
		QTabBar::mousePressEvent(event);
		emit mousePressed();
	}

	virtual void mouseMoveEvent(QMouseEvent* event) override
	{
		QTabBar::mouseMoveEvent(event);
		emit mouseMoved();
	}

	virtual void mouseReleaseEvent(QMouseEvent* event) override
	{
		QTabBar::mouseReleaseEvent(event);
		emit mouseReleased();
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
};
