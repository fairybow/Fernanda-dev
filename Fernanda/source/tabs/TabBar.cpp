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

QUuid TabBar::tabId(int index)
{
	return m_trueTabBar->tabData(index).toMap()[DATA_ID].toUuid();
}

const QString TabBar::title(int index)
{
	return m_trueTabBar->tabData(index).toMap()[DATA_TITLE].toString();
}

int TabBar::index(QUuid id)
{
	auto index = -1;
	for (auto i = 0; i < m_trueTabBar->count(); ++i)
		if (tabId(i) == id) {
			index = i;
			break;
		}
	return index;
}

bool TabBar::isUntitled()
{
	if (m_trueTabBar->currentIndex() < 0) return false;
	return title(m_trueTabBar->currentIndex()).isEmpty();
}

void TabBar::setUntitledDisplay(const QString& text)
{
	auto index = m_trueTabBar->currentIndex();
	if (index < 0 || !isUntitled()) return;
	m_trueTabBar->setTabText(index, text);
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

int TabBar::create(QUuid id, StdFsPath titlePath)
{
	blockSignals(true);
	QString title = titlePath.empty() ? QString() : Path::qStringName(titlePath);
	auto index = m_trueTabBar->addTab(title);
	setButton(index, id);
	setData(index, id, title);
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

void TabBar::setData(int index, QUuid id, QString title)
{
	QVariantMap data;
	data[DATA_ID] = id;
	data[DATA_TITLE] = title;
	m_trueTabBar->setTabData(index, data);
}
