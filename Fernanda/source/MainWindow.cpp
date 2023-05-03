#include "MainWindow.h"

MainWindow::MainWindow(const char* name, bool isDev, StdFsPath file, QWidget* parent)
	: QMainWindow(parent), m_isDev(isDev)
{
	setObjectName(name);

	/*adjustTitle();
	addWidgets();
	connections();
	shortcuts();*/
	setGeometry(0, 0, 1000, 600); // from user data
	Layout::setCentralWidget(this, m_splitter);
	setMenuBar(m_menuBar);
	setStatusBar(m_statusBar);
	m_statusBar->addPermanentWidget(m_meter, 0);

	connections();

	m_menuBar->makeSubmenus();
	
	m_splitter->initialize({ 0.2, 0.4, 0.4 }, 1);

	// testing
}

void MainWindow::connections()
{
	splitterConnections();
	treeViewConnections();
	editorConnections();
	previewConnections();
	menuBarConnections();
}

void MainWindow::splitterConnections()
{
	connect(m_splitter, &Splitter::askWindowSize, this, [&] {
		return geometry();
		});
}

void MainWindow::treeViewConnections()
{
	//
}

void MainWindow::editorConnections()
{
	//
}

void MainWindow::previewConnections()
{
	//
}

void MainWindow::menuBarConnections()
{
	connect(m_menuBar, &MenuBar::getUserDataPath, this, [&] {
		return m_user->dataFolder();
		});
	connect(m_menuBar, &MenuBar::askStyleEditor, this, [&](StdFsPath path) {
		saveConfigPassthrough(
			Path::toQString(path), "theme", m_editor, [&] {
				m_stylist->style(m_editor, path);
			});
		});
	connect(m_menuBar, &MenuBar::askStyleWindow, this, [&](StdFsPath path) {
		saveConfigPassthrough(
			Path::toQString(path), "theme", this, [&] {
				m_stylist->style(this, path);
			});
		});
}
