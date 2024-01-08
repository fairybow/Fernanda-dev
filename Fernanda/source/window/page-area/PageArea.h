#pragma once

#include "PageAreaStack.hpp"
#include "TabBar.h"
#include "TabCloseButton.hpp"
#include "TabControlBox.h"

#include <QDebug>
#include <QLabel>
#include <QMap>
#include <QPixmap>
#include <QString>
#include <QVariant>
#include <QWidget>

class PageArea : public QWidget
{
	Q_OBJECT

public:
	PageArea(QWidget* parent = nullptr);
	~PageArea() { qDebug() << __FUNCTION__; }

	int count() const;
	QWidget* widgetAt(int index) const;
	int indexOf(const QWidget* const widget) const;
	int currentIndex() const;
	QWidget* currentWidget() const;

	Qt::TextElideMode tabsElideMode() const;
	void setTabsElideMode(Qt::TextElideMode mode);
	bool tabsMovable() const;
	void setTabsMovable(bool movable);
	bool drawBarBase() const;
	void setDrawBarBase(bool draw);
	bool barExpanding() const;
	void setBarExpanding(bool enabled);
	QPixmap underlayPixmap() const;
	void setUnderlayPixmap(const QPixmap& pixmap);
	bool hasScrolls() const;
	void setHasScrolls(bool hasScrolls);
	QVariant data(int index) const;
	void setData(int index, const QVariant& data);
	bool tabFlagged(int index) const;
	void setTabFlagged(int index, bool flagged);
	QString pageTabText(int index) const;
	void setPageTabText(int index, const QString& text);

	int add(QWidget* widget, const QString& tabText);
	int insert(int index, QWidget* widget, const QString& tabText);
	QWidget* remove(int index);

public slots:
	void setCurrentIndex(int index);
	void setCurrentWidget(QWidget* widget);

private:
	TabBar* m_tabBar = new TabBar(this);
	TabControlBox* m_controls = new TabControlBox(m_tabBar, this);
	PageAreaStack* m_mainStack = new PageAreaStack;
	PageAreaStack* m_widgetsStack = new PageAreaStack;
	QLabel* m_underlay = new QLabel;

	QMap<QWidget*, QVariant> m_data;
	bool m_hasScrolls = true;

	void setup();
	void setupTabBar();

	int addNewPage(QWidget* widget, const QString& tabText, int insertIndex = -1);
	QWidget* dismantlePage(int index);
	void addTabCloseButton(int index);
	TabCloseButton* tabCloseButton(int index) const;
	void maybeShowScrolls();
	void maybeShowUnderlay();
	void removePageData(int index);

private slots:
	void onTabBarCurrentChanged(int index);

signals:
	void currentChanged(int index);
	void addRequested();
	void closeRequested(int index);
};
