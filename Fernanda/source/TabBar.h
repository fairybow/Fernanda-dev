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
	TabBar(const char* name, QWidget* parent = nullptr);

	int find(QUuid id, StdFsPath pathForTitle = StdFsPath(), bool switchTo = true);
	QUuid id(int index);

protected:
	virtual void mousePressEvent(QMouseEvent* event) override;
	virtual void mouseMoveEvent(QMouseEvent* event) override;
	virtual void mouseReleaseEvent(QMouseEvent* event) override;
	virtual void resizeEvent(QResizeEvent* event) override;
	virtual void tabInserted(int index) override;
	virtual void tabLayoutChange() override;
	virtual void tabRemoved(int index) override;

private:
	bool m_aboutToBeDragged = false;
	bool m_dragging = false;
	QToolButton* m_add = new QToolButton(this);

	void moveAddButton();
};
