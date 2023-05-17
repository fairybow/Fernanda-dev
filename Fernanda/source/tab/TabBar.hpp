#pragma once

#include "../common/Layout.hpp"
#include "../common/Path.hpp"
#include "../common/Widget.hpp"
#include "TabControlBox.hpp"
#include "TrueTabBar.hpp"

#include <QString>
#include <QToolButton>
#include <QUuid>

#include <filesystem>

class TabBar : public Widget<>
{
	using StdFsPath = std::filesystem::path;

	Q_OBJECT

public:
	TabBar(const char* name, QWidget* parent = nullptr)
		: Widget(name, parent)
	{
		nameObjects(name);
		setupWidgets();
		connections();
	}

	int serve(QUuid id, StdFsPath pathForTitle = StdFsPath(), bool switchTo = true)
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

	QUuid id(int index)
	{
		return m_trueTabBar->tabData(index).value<QUuid>();
	}

signals:
	void currentChanged(int index);
	void askNew();

private:
	TrueTabBar* m_trueTabBar = new TrueTabBar;
	QToolButton* m_floatingAdd = new QToolButton(m_trueTabBar);
	TabControlBox* m_controlBox = new TabControlBox(m_trueTabBar);

	bool m_aboutToBeDragged = false;

	void nameObjects(const char* name)
	{
		m_trueTabBar->setObjectName(name);
		auto control_name = name + QString("-control");
		m_controlBox->setObjectName(control_name);
		m_floatingAdd->setObjectName(control_name);
	}

	void setupWidgets()
	{
		m_floatingAdd->setText("+");
		auto layout = Layout::box(Layout::Line::Horizontally, { m_trueTabBar, m_controlBox }, this);
		layout->setStretch(0, 1);
		layout->setStretch(1, 0);
		Layout::setUniformSpacing(layout, 0);
	}

	void connections()
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

	void hideControls()
	{
		m_floatingAdd->setVisible(false);
		m_controlBox->setVisible(false);
	}

	void showControls()
	{
		m_controlBox->setVisible(isFull());
		m_floatingAdd->setVisible(!m_controlBox->isVisible());
	}

	bool isFull()
	{
		auto tabs_width = 0;
		for (auto i = 0; i < m_trueTabBar->count(); ++i)
			tabs_width += m_trueTabBar->tabRect(i).width();
		return (tabs_width > m_trueTabBar->width());
	}

	void adjustControls()
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
};
