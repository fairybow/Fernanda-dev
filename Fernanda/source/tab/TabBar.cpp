#include "TabBar.h"

TabBar::TabBar(const char* name, QWidget* parent)
	: Widget(name, parent)
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
	auto control_name = name + QString("-control");
	m_controlBox->setObjectName(control_name);
	m_floatingAdd->setObjectName(control_name);
}

void TabBar::setupWidgets()
{
	m_floatingAdd->setText("+");
	auto layout = Layout::box(Layout::Line::Horizontally, { m_trueTabBar, m_controlBox }, this);
	layout->setStretch(0, 1);
	layout->setStretch(1, 0);
	Layout::setUniformSpacing(layout, 0);
}

void TabBar::connections()
{
	connect(m_floatingAdd, &QToolButton::clicked, this, [&] {
		emit askNew();
		});
	connect(m_controlBox, &TabControlBox::addTabClicked, this, [&] {
		emit askNew();
		});

	connect(m_trueTabBar, &TrueTabBar::currentChanged, this, [&](int index) {
		emit currentChanged(index);
		});
	connect(m_trueTabBar, &TrueTabBar::mousePressed, this, [&] {
		m_aboutToBeDragged = true;
		});
	connect(m_trueTabBar, &TrueTabBar::mouseMoved, this, [&] {
		if (m_aboutToBeDragged)
			hideControls();
		});
	connect(m_trueTabBar, &TrueTabBar::mouseReleased, this, [&] {
		m_aboutToBeDragged = false;
		showControls();
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

void TabBar::hideControls()
{
	m_controlBox->setVisible(false);
	m_floatingAdd->setVisible(false);
}

void TabBar::showControls()
{
	m_controlBox->setVisible(isFull());
	m_floatingAdd->setVisible(!m_controlBox->isVisible());
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
	Utility::delayCall(this, [&] {
		if (m_trueTabBar->count() < 1) return;

		showControls();

		auto rect = m_trueTabBar->tabRect(m_trueTabBar->count() - 1);
		auto x = rect.right() + 3;
		auto y = rect.center().y() - (m_floatingAdd->height() / 2) + 1;
		auto max_x = m_trueTabBar->width() - m_floatingAdd->width();
		if (x > max_x)
			x = max_x;
		m_floatingAdd->move(x, y);
		});
}
