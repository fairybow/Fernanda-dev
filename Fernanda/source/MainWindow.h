#pragma once

#include "common/Layout.hpp"
#include "editor/Editor.hpp"
#include "Indicator.h"
#include "menu/MenuBar.h"
#include "Meter.h"
#include "preview/Preview.hpp"
#include "splitter/Splitter.h"
#include "StatusBar.hpp"
#include "Stylist.h"
#include "tree/TreeView.h"
#include "user/User.hpp"

#include <QMainWindow>
#include <QString>

#include <filesystem>

class MainWindow : public QMainWindow
{
	using StdFsPath = std::filesystem::path;

	Q_OBJECT

public:
	MainWindow(const char* name, bool isDev = false, StdFsPath file = StdFsPath(), QWidget* parent = nullptr);

signals:
	void testSignal1();
	void testSignal2();
	void testSignal3(int i);

private:
	MenuBar* m_menuBar = new MenuBar("MenuBar", m_isDev, this);
	StatusBar* m_statusBar = new StatusBar("StatusBar", this);
	Indicator* m_indicator = new Indicator("Indicator", this);
	TreeView* m_treeView = new TreeView("TreeView");
	Editor* m_editor = new Editor("Editor");
	Preview* m_preview = new Preview("Preview");
	Splitter* m_splitter = new Splitter("Splitter", Qt::Horizontal,
		{ m_treeView, m_editor, m_preview }, this);
	Meter* m_meter = new Meter("Meter");
	Stylist* m_stylist = new Stylist({ this, m_editor }, this);
	User* m_user = new User(QCoreApplication::applicationName(), this);

	const bool m_isDev;

	void connections();
	void splitterConnections();
	void treeViewConnections();
	void editorConnections();
	void previewConnections();
	void menuBarConnections();
};
