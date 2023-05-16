#pragma once

#include "../common/Path.hpp"
#include "../common/Widget.hpp"
#include "TabControl.hpp"

#include <QMouseEvent>
#include <QResizeEvent>
#include <QSize>
#include <QTabBar>
#include <QUuid>

#include <filesystem>

class TabBar : public Widget<QTabBar>
{
	using StdFsPath = std::filesystem::path;

	Q_OBJECT

public:
	TabBar(const char* name, QWidget* parent = nullptr);

	int serve(QUuid id, StdFsPath pathForTitle = StdFsPath(), bool switchTo = true);
	QUuid id(int index);

signals:
	void askNew();

protected:
	virtual void mousePressEvent(QMouseEvent* event) override;
	virtual void mouseMoveEvent(QMouseEvent* event) override;
	virtual void mouseReleaseEvent(QMouseEvent* event) override;
	virtual void resizeEvent(QResizeEvent* event) override;
	virtual void tabInserted(int index) override;
	virtual void tabRemoved(int index) override;

private:
	bool m_aboutToBeDragged = false;
	TabControl* m_controller = new TabControl(this);

	void setupControls(const char* name);
	bool isFull();
};
