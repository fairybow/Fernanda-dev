#pragma once

#include "common/Layout.hpp"
#include "editor/Editor.hpp"
#include "preview/Preview.hpp"
#include "splitter/Splitter.h"
#include "tree/Tree.hpp"

#include <QMainWindow>
#include <QMenuBar>
#include <QStatusBar>

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(const char* name, QWidget* parent = nullptr);

private:
	QMenuBar* m_menuBar = new QMenuBar(this);
	QStatusBar* m_statusBar = new QStatusBar(this);
	Tree* m_tree = new Tree("Tree");
	Editor* m_editor = new Editor("Editor");
	Preview* m_preview = new Preview("Preview");
	Splitter* m_splitter = new Splitter("Splitter", Qt::Horizontal, { m_tree, m_editor, m_preview }, this);
};
