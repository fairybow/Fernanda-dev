#include "MainWindow.h"

MainWindow::MainWindow(const char* name, bool isDev, StdFsPath file, QWidget* parent)
	: Widget(name, parent), m_isDev(isDev)
{
	/*adjustTitle();
	addWidgets();
	connections();
	shortcuts();*/
	setGeometry(0, 0, 1000, 600); // from user data
	Layout::setCentralWidget(this, m_splitter); // addWidgets();
	setMenuBar(m_menuBar); // addWidgets();
	setStatusBar(m_statusBar); // addWidgets();
	m_statusBar->addPermanentWidget(m_meter, 0); // addWidgets();

	connections();

	m_menuBar->makeSubmenus();
	
	m_splitter->initialize({ 0.2, 0.4, 0.4 }, 1);

	// testing

	loadConfig();

	/*auto button_1 = new QPushButton;
	button_1->setText("Test");
	m_statusBar->addPermanentWidget(button_1, 0);
	connect(button_1, &QPushButton::pressed, this, [&] { loadConfig(); });*/
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

void MainWindow::loadConfig()
{
	// etc.
	loadMenuBarConfig();
}

void MainWindow::loadMenuBarConfig()
{
	auto editor_theme = m_user->load("theme", m_editor, Path::toQString(m_menuBar->defaultEditorTheme()));
	auto window_theme = m_user->load("theme", this, Path::toQString(m_menuBar->defaultWindowTheme()));
	auto fs_editor_theme = Path::toStdFs(editor_theme);
	auto fs_window_theme = Path::toStdFs(window_theme);
	m_stylist->style(m_editor, fs_editor_theme);
	m_stylist->style(this, fs_window_theme);
	m_menuBar->setSelectedEditorTheme(fs_editor_theme);
	m_menuBar->setSelectedWindowTheme(fs_window_theme);
}
