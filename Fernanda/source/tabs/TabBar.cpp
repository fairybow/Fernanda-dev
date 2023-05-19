#include "TabBar.h"

TabBar::TabBar(const char* name, int minTabSize, int maxTabSize, QWidget* parent)
	: Widget(name, parent), m_trueTabBar(new TrueTabBar(minTabSize, maxTabSize))
{
	nameObjects(name);
	setupWidgets();
	connections();
}

int TabBar::serve(QUuid id, StdFsPath pathForTitle, bool switchTo)
{
	auto index = -1;
	for (auto i = 0; i < m_trueTabBar->count(); ++i) {
		if (m_trueTabBar->tabData(i) == id) {
			index = i;
			break;
		}
	}
	if (index == -1) {
		blockSignals(true);
		index = m_trueTabBar->addTab(
			pathForTitle.empty() ? QString() : Path::qStringName(pathForTitle));
		m_trueTabBar->setTabButton(index, QTabBar::ButtonPosition::RightSide, closeButton(id));
		m_trueTabBar->setTabData(index, id);
		blockSignals(false);
	}
	if (switchTo)
		m_trueTabBar->setCurrentIndex(index);
	return index;
}

QUuid TabBar::id(int index)
{
	return m_trueTabBar->tabData(index).value<QUuid>();
}

void TabBar::nameObjects(const char* name)
{
	m_trueTabBar->setObjectName(name);
	m_controlBox->setObjectName(name);
	//m_controlBox->setObjectName(name + QString("-control"));
}

void TabBar::setupWidgets()
{
	auto layout = Layout::box(Layout::Line::Horizontally, { m_controlBox, m_trueTabBar }, this);
	layout->setStretch(0, 0);
	layout->setStretch(1, 1);
	Layout::setUniformSpacing(layout, 0);
}

void TabBar::connections()
{
	connect(m_controlBox, &TabControlBox::addTabClicked, this, [&] {
		emit askNew();
		});

	connect(m_trueTabBar, &TrueTabBar::currentChanged, this, [&](int index) {
		emit currentChanged(index);
		});
	connect(m_trueTabBar, &TrueTabBar::resized, this, [&] {
		adjustControls();
		});
	connect(m_trueTabBar, &TrueTabBar::inserted, this, [&] {
		adjustControls();
		});
	connect(m_trueTabBar, &TrueTabBar::removed, this, [&] {
		adjustControls();
		});
}

bool TabBar::isFull()
{
	auto tabs_width = 0;
	for (auto i = 0; i < m_trueTabBar->count(); ++i)
		tabs_width += m_trueTabBar->tabRect(i).width();
	return (tabs_width > m_trueTabBar->width());
}

void TabBar::adjustControls()
{
	m_controlBox->setScrollerVisible(isFull());
}

QToolButton* TabBar::closeButton(QUuid id)
{
	auto close_button = new QToolButton;
	close_button->setText("x");
	connect(close_button, &QToolButton::clicked, this, [&, id] {
		for (auto i = 0; i < m_trueTabBar->count(); ++i)
			if (m_trueTabBar->tabData(i) == id) {
				emit m_trueTabBar->tabCloseRequested(i);
				break;
			}
		});
	return close_button;
}
