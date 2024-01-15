#pragma once

#include "../../../common/UiButton.hpp"

#include <QDebug>
#include <QTabBar>
#include <QWheelEvent>
#include <QWidget>

class TabControlBox : public QWidget
{
	Q_OBJECT

public:
	TabControlBox(QTabBar* tabBar, QWidget* parent = nullptr);
	~TabControlBox() { qDebug() << __FUNCTION__; }

	void setShowScrolls(bool visible);

protected:
	void wheelEvent(QWheelEvent* event) override;

private:
	QTabBar* m_tabBar;
	UiButton* m_addTab = new UiButton(UiButton::Ui::Add, this);
	UiButton* m_scrollLeft = new UiButton(UiButton::Ui::ChevronLeft, this);
	UiButton* m_scrollRight = new UiButton(UiButton::Ui::ChevronRight, this);

	void setup();
	void sizing();
	void connections();

private slots:
	void onScrollLeftClicked();
	void onScrollRightClicked();

signals:
	void addTabClicked();
};
