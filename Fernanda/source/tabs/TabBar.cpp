#include "TabBar.h"

TabBar::TabBar(const char* name, int minTabSize, int maxTabSize, QWidget* parent)
	: Widget(name, parent),
	m_trueTabBar(new TrueTabBar(minTabSize, maxTabSize))
{
	m_trueTabBar->setObjectName(name);
	setupWidgets();
	connections();
}

int TabBar::serve(QUuid id, StdFsPath pathForTitle, bool switchTo)
{
	auto next_index = indexById(id);
	if (next_index == -1)
		next_index = create(id, pathForTitle);
	if (switchTo)
		m_trueTabBar->setCurrentIndex(next_index);
	return next_index;
}

QUuid TabBar::idByIndex(int index)
{
	return m_trueTabBar->tabData(index).toMap()[DATA_ID].toUuid();
}

int TabBar::indexById(QUuid id)
{
	auto index = -1;
	for (auto i = 0; i < m_trueTabBar->count(); ++i)
		if (idByIndex(i) == id) {
			index = i;
			break;
		}
	return index;
}

const QString TabBar::title(int index)
{
	return m_trueTabBar->tabData(index).toMap()[DATA_TITLE].toString();
}

bool TabBar::isUntitled()
{
	if (m_trueTabBar->currentIndex() < 0) return false;
	return title(m_trueTabBar->currentIndex()).isEmpty();
}

void TabBar::setUntitledDisplay(const QString& text, int charLimit)
{
	auto index = m_trueTabBar->currentIndex();
	if (index < 0 || !isUntitled()) return;
	m_trueTabBar->setTabText(index, text.left(charLimit));
}

void TabBar::updateEditedState(QUuid id, bool edited)
{
	auto changed_index = indexById(id);
	if (changed_index == -1) return;
	auto button = qobject_cast<CloseTab*>(
		m_trueTabBar->tabButton(changed_index, QTabBar::RightSide));
	if (button)
		button->setEdited(edited);
}

void TabBar::setupWidgets()
{
	auto control_box = Layout::box(Layout::Line::Horizontally, { m_add, m_scrollLeft, m_scrollRight });
	control_box->setContentsMargins(0, 0, 4, 0);
	auto layout = Layout::box(Layout::Line::Horizontally, nullptr, this);
	layout->addLayout(control_box, 0);
	layout->addWidget(m_trueTabBar, 1);
	Layout::setUniformSpacing(layout, 0);
}

void TabBar::connections()
{
	connect(m_add, &AddTab::clicked, this, [&] {
		emit askAdd();
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
	auto visible = isFull();
	m_scrollLeft->setVisible(visible);
	m_scrollRight->setVisible(visible);
	layout()->update();
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
	auto button = new CloseTab(this);
	connect(button, &CloseTab::clicked, this, [&, id] {
		emit askClose(id);
		// close tab via id
		// delete button after closing tab
		});
	m_trueTabBar->setTabButton(index, QTabBar::ButtonPosition::RightSide, button);
}

void TabBar::setData(int index, QUuid id, QString title)
{
	QVariantMap data;
	data[DATA_ID] = id;
	data[DATA_TITLE] = title;
	m_trueTabBar->setTabData(index, data);
}
