#pragma once

#include "common/Layout.hpp"
#include "editor/Editor.hpp"
#include "Indicator.h"
#include "menu/MenuBar.h"
#include "Meter.h"
#include "preview/Preview.hpp"
#include "splitter/Splitter.h"
#include "StatusBar.hpp"
#include "tree/Tree.h"

#include <QMainWindow>

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget* parent = nullptr);

private:
	MenuBar* m_menuBar = new MenuBar(this);
	StatusBar* m_statusBar = new StatusBar(this);
	Indicator* m_indicator = new Indicator(this);
	Tree* m_tree = new Tree;
	Editor* m_editor = new Editor;
	Preview* m_preview = new Preview;
	Splitter* m_splitter = new Splitter(Qt::Horizontal, { m_tree, m_editor, m_preview }, this);
	Meter* m_meter = new Meter;
};
