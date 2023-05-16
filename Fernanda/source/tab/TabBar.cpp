#include "TabBar.h"

TabBar::TabBar(const char* name, QWidget* parent)
	: Widget(name, parent)
{
	//setAutoHide(false);
	setTabsClosable(true);
	setMovable(true);
	setExpanding(false);
	setUsesScrollButtons(true);
	setupControls(name);
}

int TabBar::serve(QUuid id, StdFsPath pathForTitle, bool switchTo)
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

void TabBar::mousePressEvent(QMouseEvent* event)
{
	QTabBar::mousePressEvent(event);
	m_aboutToBeDragged = true;
}

void TabBar::mouseMoveEvent(QMouseEvent* event)
{
	QTabBar::mouseMoveEvent(event);
	if (m_aboutToBeDragged)
		m_controller->setVisible(false);
}

void TabBar::mouseReleaseEvent(QMouseEvent* event)
{
	QTabBar::mouseReleaseEvent(event);
	m_aboutToBeDragged = false;
	m_controller->setVisible(true);
}

void TabBar::resizeEvent(QResizeEvent* event)
{
	QTabBar::resizeEvent(event);
	m_controller->adjust();
}

void TabBar::tabInserted(int index)
{
	QTabBar::tabInserted(index);
	m_controller->adjust();
}

void TabBar::tabRemoved(int index)
{
	QTabBar::tabRemoved(index);
	m_controller->adjust();
}

void TabBar::setupControls(const char* name)
{	
	connect(m_controller, &TabControl::addTabClicked, this, [&] {
		emit askNew();
		});
}

bool TabBar::isFull()
{
	auto tabs_width = 0;
	for (auto i = 0; i < count(); ++i)
		tabs_width += tabRect(i).width();
	return (tabs_width > width());
}
