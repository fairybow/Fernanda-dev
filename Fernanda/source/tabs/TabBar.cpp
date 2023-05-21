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
	auto next_index = index(id);
	if (next_index == -1) {
		blockSignals(true);
		next_index = m_trueTabBar->addTab(
			pathForTitle.empty() ? QString() : Path::qStringName(pathForTitle));
		m_trueTabBar->setTabButton(next_index, QTabBar::ButtonPosition::RightSide, closeButton(id));
		m_trueTabBar->setTabData(next_index, id);
		blockSignals(false);
	}
	if (switchTo)
		m_trueTabBar->setCurrentIndex(next_index);
	return next_index;
}

QUuid TabBar::id(int index)
{
	return m_trueTabBar->tabData(index).value<QUuid>();
}

int TabBar::index(QUuid id)
{
	auto index = -1;
	for (auto i = 0; i < m_trueTabBar->count(); ++i)
		if (m_trueTabBar->tabData(i) == id) {
			index = i;
			break;
		}
	return index;
}

void TabBar::updateEditedState(QUuid id, bool edited)
{
	auto changed_index = index(id);
	if (changed_index == -1) return;
	//m_tabEditedStates[index] = edited;
	if (edited) {
		//show flag
	}
	else {
		//hide flag
	}
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
