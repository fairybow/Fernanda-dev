#include "../common/Layout.hpp"
#include "PageArea.h"

#include <QList>
#include <QVariant>

constexpr char HIDE_DEFAULT_SCROLLERS[] = "TabBar::scroller{ width: 0px; }";

PageArea::PageArea(QWidget* parent)
	: QWidget(parent)
{
	setup();
}

int PageArea::count() const
{
	return m_tabBar->count();
}

QWidget* PageArea::widgetAt(int index) const
{
	auto tab_data = m_tabBar->tabData(index);

	return tab_data.value<QWidget*>();
}

int PageArea::indexOf(const QWidget* const widget) const
{
	for (auto i = 0; i < count(); ++i)
		if (widgetAt(i) == widget)
			return i;

	return -1;
}

int PageArea::currentIndex() const
{
	return m_tabBar->currentIndex();
}

QWidget* PageArea::currentWidget() const
{
	return widgetAt(currentIndex());
}

Qt::TextElideMode PageArea::tabsElideMode() const
{
	return m_tabBar->elideMode();
}

void PageArea::setTabsElideMode(Qt::TextElideMode mode)
{
	m_tabBar->setElideMode(mode);
}

bool PageArea::tabsMovable() const
{
	return m_tabBar->isMovable();
}

void PageArea::setTabsMovable(bool movable)
{
	m_tabBar->setMovable(movable);
}

bool PageArea::drawBarBase() const
{
	return m_tabBar->drawBase();
}

void PageArea::setDrawBarBase(bool draw)
{
	m_tabBar->setDrawBase(draw);
}

bool PageArea::barExpanding() const
{
	return m_tabBar->expanding();
}

void PageArea::setBarExpanding(bool enabled)
{
	m_tabBar->setExpanding(enabled);
}

QPixmap PageArea::underlayPixmap() const
{
	return m_underlay->pixmap();
}

void PageArea::setUnderlayPixmap(const QPixmap& pixmap)
{
	m_underlay->setPixmap(pixmap);
}

bool PageArea::hasScrolls() const
{
	return m_hasScrolls;
}

void PageArea::setHasScrolls(bool hasScrolls)
{
	m_hasScrolls = hasScrolls;

	maybeShowScrolls();
}

QVariant PageArea::data(int index) const
{
	auto widget = widgetAt(index);

	return m_data.value(widget);
}

void PageArea::setData(int index, const QVariant& data)
{
	auto widget = widgetAt(index);
	m_data.insert(widget, data);
}

bool PageArea::tabFlagged(int index) const
{
	return tabCloseButton(index)->flagged();
}

void PageArea::setTabFlagged(int index, bool flagged)
{
	tabCloseButton(index)->setFlagged(flagged);
}

QString PageArea::pageTabText(int index) const
{
	return m_tabBar->tabText(index);
}

void PageArea::setPageTabText(int index, const QString& text)
{
	m_tabBar->setTabText(index, text);
}

int PageArea::add(QWidget* widget, const QString& tabText)
{
	auto index = addNewPage(widget, tabText);

	return index;
}

int PageArea::insert(int index, QWidget* widget, const QString& tabText)
{
	return addNewPage(widget, tabText, index);
}

QWidget* PageArea::remove(int index)
{
	removePageData(index);

	return dismantlePage(index);
}

void PageArea::setCurrentIndex(int index)
{
	m_tabBar->setCurrentIndex(index);
}

void PageArea::setCurrentWidget(QWidget* widget)
{
	auto index = indexOf(widget);

	setCurrentIndex(index);
}

void PageArea::setup()
{
	QObjectList top_widgets{ m_controls, m_tabBar };
	auto top_layout = Layout::box(Box::Horizontal, top_widgets);
	QObjectList layouts{ top_layout, m_mainStack };
	Layout::box(Box::Vertical, this, layouts);

	m_underlay->setAlignment(Qt::AlignCenter);
	m_mainStack->addWidget(m_underlay);
	m_mainStack->addWidget(m_widgetsStack);

	setupTabBar();

	connect(m_controls, &TabControlBox::addTabClicked, this, [&] { emit addRequested(); });
}

void PageArea::setupTabBar()
{
	m_tabBar->setAutoHide(false);
	m_tabBar->setTabsClosable(false);
	m_tabBar->setUsesScrollButtons(true);
	m_tabBar->setStyleSheet(HIDE_DEFAULT_SCROLLERS);
	m_tabBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	connect(m_tabBar, &TabBar::currentChanged, this, &PageArea::onTabBarCurrentChanged);

	connect(m_tabBar, &TabBar::resized, this, [&] { maybeShowScrolls(); });
	connect(m_tabBar, &TabBar::tabCountChanged, this, [&] { maybeShowScrolls(); });
	connect(m_tabBar, &TabBar::tabCountChanged, this, [&] { maybeShowUnderlay(); });
}

int PageArea::addNewPage(QWidget* widget, const QString& tabText, int insertIndex)
{
	m_widgetsStack->addWidget(widget);

	auto index = m_tabBar->insertTab(insertIndex, tabText);
	m_tabBar->setTabData(index, QVariant::fromValue(widget));

	addTabCloseButton(index);

	// Note:
	// `m_tabBar` is automatically switched in the case of adding the first tab,
	// which causes `onTabBarCurrentChanged` to be called before `setTabData` has
	// been called, which means the slot returns early. So, in the case of
	// adding the first tab, the slot must be manually called with `index` 0.
	if (count() == 1)
		onTabBarCurrentChanged(0);

	return index;
}

QWidget* PageArea::dismantlePage(int index)
{
	auto widget = widgetAt(index);
	m_widgetsStack->removeWidget(widget);
	auto close_button = tabCloseButton(index);

	m_tabBar->removeTab(index);

	if (close_button)
		delete close_button;

	return widget; // Does widget need to be reparented?
}

void PageArea::addTabCloseButton(int index)
{
	auto close_button = new TabCloseButton(m_tabBar);
	m_tabBar->setTabButton(index, TabBar::RightSide, close_button);

	auto slot = [&](int close_button_index) { emit closeRequested(close_button_index); };
	connect(close_button, &TabCloseButton::clickedAt, this, slot);
}

TabCloseButton* PageArea::tabCloseButton(int index) const
{
	auto close_button = m_tabBar->tabButton(index, TabBar::RightSide);

	return qobject_cast<TabCloseButton*>(close_button);
}

void PageArea::maybeShowScrolls()
{
	auto needs_scrolls = m_hasScrolls ? m_tabBar->isFull() : false;
	m_controls->setShowScrolls(needs_scrolls);
}

void PageArea::maybeShowUnderlay()
{
	m_widgetsStack->count()
		? m_mainStack->setCurrentWidget(m_widgetsStack)
		: m_mainStack->setCurrentWidget(m_underlay);
}

void PageArea::removePageData(int index)
{
	auto widget = widgetAt(index);
	m_data.remove(widget);
}

void PageArea::onTabBarCurrentChanged(int index)
{
	if (index < 0) {
		// In the event the last tab is removed, we still need
		// to emit the signal with an `index` of `-1`.
		emit currentChanged(index);
		return;
	}

	auto widget = widgetAt(index);
	if (!widget) return;

	m_widgetsStack->setCurrentWidget(widget);

	emit currentChanged(index);
}
