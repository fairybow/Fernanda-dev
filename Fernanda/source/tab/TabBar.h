#pragma once

#include "../common/Layout.hpp"
#include "../common/Path.hpp"
#include "../common/Utility.hpp"
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
	TabBar(const char* name, QWidget* parent = nullptr);

	int serve(QUuid id, StdFsPath pathForTitle = StdFsPath(), bool switchTo = true);
	QUuid id(int index);

signals:
	void currentChanged(int index);
	void askNew();

private:
	TrueTabBar* m_trueTabBar = new TrueTabBar;
	QToolButton* m_floatingAdd = new QToolButton(m_trueTabBar);
	TabControlBox* m_controlBox = new TabControlBox(m_trueTabBar);

	bool m_aboutToBeDragged = false;

	void nameObjects(const char* name);
	void setupWidgets();
	void connections();
	void hideControls();
	void showControls();
	bool isFull();
	void adjustControls();
};
