#pragma once

#include "common/Path.hpp"
#include "common/Widget.hpp"

#include <QMouseEvent>
#include <QResizeEvent>
#include <QSize>
#include <QTabBar>
#include <QToolButton>
#include <QUuid>

#include <filesystem>

class TabBar : public Widget<QTabBar>
{
	using StdFsPath = std::filesystem::path;

public:
	TabBar(const char* name, QWidget* parent = nullptr)
		: Widget(name, parent)
	{
		m_add->setText("+");
		//setAutoHide(true);
		setTabsClosable(true);
		setMovable(true);
		setExpanding(false);
	}

	int find(QUuid id, StdFsPath pathForTitle = StdFsPath(), bool switchTo = true)
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

	QUuid id(int index)
	{
		return tabData(index).value<QUuid>();
	}

protected:
	virtual void resizeEvent(QResizeEvent* event) override
	{
		QTabBar::resizeEvent(event);
		moveAddButton();
	}

	virtual void tabInserted(int index) override
	{
		QTabBar::tabInserted(index);
		moveAddButton();
	}

	virtual void tabLayoutChange() override
	{
		QTabBar::tabLayoutChange();
		moveAddButton();
	}

	virtual void tabRemoved(int index) override
	{
		QTabBar::tabRemoved(index);
		moveAddButton();
	}

	virtual void mousePressEvent(QMouseEvent* event) override
	{
		QTabBar::mousePressEvent(event);
		m_aboutToBeDragged = true;
	}

	virtual void mouseMoveEvent(QMouseEvent* event) override
	{
		QTabBar::mouseMoveEvent(event);
		if (m_aboutToBeDragged)
			m_dragging = true;
		if (m_dragging)
			m_add->setVisible(false);
	}

	virtual void mouseReleaseEvent(QMouseEvent* event) override
	{
		QTabBar::mouseReleaseEvent(event);
		m_aboutToBeDragged = false;
		m_dragging = false;
		m_add->setVisible(true);
	}

private:
	bool m_aboutToBeDragged = false;
	bool m_dragging = false;
	QToolButton* m_add = new QToolButton(this);

	void moveAddButton()
	{
		if (count() < 1) return;
		auto rect = tabRect(count() - 1);
		auto x = rect.right() + 3;
		auto y = rect.center().y() - (m_add->height() / 2);
		m_add->move(x, y);
	}
};
