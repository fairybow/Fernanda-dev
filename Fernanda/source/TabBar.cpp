#include "TabBar.h"

TabBar::TabBar(const char* name, QWidget* parent)
	: Widget(name, parent)
{
	m_add->setText("+");
	//setAutoHide(true);
	setTabsClosable(true);
	setMovable(true);
	setExpanding(false);
}

int TabBar::find(QUuid id, StdFsPath pathForTitle, bool switchTo)
{
	auto index = -1;
	for (auto i = 0; i < count(); ++i) {
		if (tabData(i) == id) {
			index = i;
			break;
		}
	}
	if (index == -1) {
		blockSignals(true);
		index = addTab(
			pathForTitle.empty() ? QString() : Path::qStringName(pathForTitle));
		setTabData(index, id);
		blockSignals(false);
	}
	if (switchTo)
		setCurrentIndex(index);
	return index;
}

QUuid TabBar::id(int index)
{
	return tabData(index).value<QUuid>();
}

void TabBar::resizeEvent(QResizeEvent* event)
{
	QTabBar::resizeEvent(event);
	moveAddButton();
}

void TabBar::tabInserted(int index)
{
	QTabBar::tabInserted(index);
	moveAddButton();
}

void TabBar::tabLayoutChange()
{
	QTabBar::tabLayoutChange();
	moveAddButton();
}

void TabBar::tabRemoved(int index)
{
	QTabBar::tabRemoved(index);
	moveAddButton();
}

void TabBar::mousePressEvent(QMouseEvent* event)
{
	QTabBar::mousePressEvent(event);
	m_aboutToBeDragged = true;
}

void TabBar::mouseMoveEvent(QMouseEvent* event)
{
	QTabBar::mouseMoveEvent(event);
	if (m_aboutToBeDragged)
		m_dragging = true;
	if (m_dragging)
		m_add->setVisible(false);
}

void TabBar::mouseReleaseEvent(QMouseEvent* event)
{
	QTabBar::mouseReleaseEvent(event);
	m_aboutToBeDragged = false;
	m_dragging = false;
	m_add->setVisible(true);
}

void TabBar::moveAddButton()
{
	if (count() < 1) return;
	auto rect = tabRect(count() - 1);
	auto x = rect.right() + 3;
	auto y = rect.center().y() - (m_add->height() / 2);
	m_add->move(x, y);
}
