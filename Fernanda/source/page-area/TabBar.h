#pragma once

#include <QDebug>
#include <QResizeEvent>
#include <QTabBar>

class TabBar : public QTabBar
{
	Q_OBJECT

public:
	TabBar(QWidget* parent = nullptr);
	~TabBar() { qDebug() << __FUNCTION__; }

	bool isFull() const;
	bool isEmpty() const;

protected:
	void resizeEvent(QResizeEvent* event) override;
	void tabInserted(int index) override;
	void tabRemoved(int index) override;

private:
	int tabsWidth() const;

signals:
	void resized();
	void tabCountChanged();
};
