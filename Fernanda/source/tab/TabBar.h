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
	TabBar(const char* name, int minTabSize = 25, int maxTabSize = 100, QWidget* parent = nullptr);

	int serve(QUuid id, StdFsPath pathForTitle = StdFsPath(), bool switchTo = true);
	QUuid id(int index);

signals:
	void currentChanged(int index);
	void askNew();

private:
	TrueTabBar* m_trueTabBar;
	TabControlBox* m_controlBox = new TabControlBox(m_trueTabBar);

	void nameObjects(const char* name);
	void setupWidgets();
	void connections();
	bool isFull();
	void adjustControls();
};
