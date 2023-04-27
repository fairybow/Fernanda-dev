#pragma once

#include "common/Layout.hpp"
#include "editor/Editor.hpp"
#include "Indicator.hpp"
#include "menu/MenuBar.hpp"
#include "Meter.hpp"
#include "preview/Preview.hpp"
#include "splitter/Splitter.h"
#include "StatusBar.hpp"
#include "tree/Tree.hpp"

#include <QMainWindow>

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(const char* name, QWidget* parent = nullptr);

private:
	MenuBar* m_menuBar = new MenuBar("MenuBar", this);
	StatusBar* m_statusBar = new StatusBar("StatusBar", this);
	Indicator* m_indicator = new Indicator("Indicator", this);
	Tree* m_tree = new Tree("Tree");
	Editor* m_editor = new Editor("Editor");
	Preview* m_preview = new Preview("Preview");
	Splitter* m_splitter = new Splitter("Splitter", Qt::Horizontal, { m_tree, m_editor, m_preview }, this);
	Meter* m_meter = new Meter("Meter", m_statusBar);
};
