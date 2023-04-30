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

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(const char* name, QWidget* parent = nullptr);

signals:
	void testSignal1();
	void testSignal2(bool checked);

private:
	MenuBar* m_menuBar = new MenuBar("MenuBar", this);
	StatusBar* m_statusBar = new StatusBar("StatusBar", this);
	Indicator* m_indicator = new Indicator("Indicator", this);
	TreeView* m_treeView = new TreeView("TreeView");
	Editor* m_editor = new Editor("Editor");
	Preview* m_preview = new Preview("Preview");
	Splitter* m_splitter = new Splitter("Splitter", Qt::Horizontal, { m_treeView, m_editor, m_preview }, this);
	Meter* m_meter = new Meter("Meter");
	Stylist* m_stylist = new Stylist({ this, m_editor }, this);
	User* m_user = new User(QCoreApplication::applicationName(), this);

	void connections();
	void treeViewConnections();
	void editorConnections();
	void previewConnections();

	// testing
	template<typename T>
	inline void emitAndSave(void (MainWindow::* signal)(T), T value)
	{
		emit (this->*signal)(value);
		m_user->save(value);
	}

};
