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
	if (next_index == -1)
		next_index = create(id, pathForTitle);
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
	auto button = qobject_cast<TabButton*>(
		m_trueTabBar->tabButton(changed_index, QTabBar::RightSide));
	if (button)
		button->setEdited(edited);
}

void TabBar::nameObjects(const char* name)
{
	m_trueTabBar->setObjectName(name);
	m_controlBox->setObjectName(name);
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

int TabBar::create(QUuid id, StdFsPath pathForTitle)
{
	blockSignals(true);

	auto index = m_trueTabBar->addTab(
		pathForTitle.empty() ? QString() : Path::qStringName(pathForTitle));
	setButton(index, id);
	m_trueTabBar->setTabData(index, id);

	blockSignals(false);
	return index;
}

void TabBar::setButton(int index, QUuid id)
{
	auto button = new TabButton(id, this);
	connect(button, &TabButton::askClose, this, [&](QUuid closing_id) {
		// close tab
		});
	// delete button after closing tab
	m_trueTabBar->setTabButton(index, QTabBar::ButtonPosition::RightSide, button);
}
