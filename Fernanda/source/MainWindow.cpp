#include "MainWindow.h"

MainWindow::MainWindow(const char* name, bool isDev, StdFsPath file, QWidget* parent)
	: Widget(name, parent), m_isDev(isDev)
{
	setupWidgets();
	connections();
	loadConfigs();

	// testing

	/*auto button_1 = new QPushButton;
	button_1->setText("Test");
	m_statusBar->addPermanentWidget(button_1, 0);
	connect(button_1, &QPushButton::pressed, this, [&] { loadConfig(); });*/
}

void MainWindow::closeEvent(QCloseEvent* event)
{
	auto state = windowState();
	showNormal();
	// return if canceled
	closeEventConfigs(state);
	event->accept();
}

void MainWindow::setupWidgets()
{
	setMenuBar(m_menuBar);
	setStatusBar(m_statusBar);
	m_statusBar->addPermanentWidget(m_meter, 0);
	m_menuBar->makeSubmenus();
	m_splitter->initialize({ 0.2, 0.4, 0.4 }, 1);
	Layout::setCentralWidget(this, m_splitter);
}

void MainWindow::connections()
{
	splitterConnections();
	treeViewConnections();
	editorConnections();
	previewConnections();
	menuBarConnections();
	menuBarConfigConnections();
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
}

void MainWindow::menuBarConfigConnections()
{
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
	connect(m_menuBar, &MenuBar::askChangeFont, this, [&](const QFont& font) {
		saveConfigPassthrough(
			font, "font", m_editor, [&] {
				m_editor->setFont(font);
			});
		});

	connect(m_menuBar, &MenuBar::getUserFont, this, [&] {
		return loadConfig<QFont>("font", m_editor, m_editor->defaulFont());
		});
}

void MainWindow::loadConfigs()
{
	//auto state = loadConfig("state", this, Qt::WindowState::WindowNoState);
	//setWindowState(state); // behaves strangely, Windows issue I think
	auto geometry = loadConfig("geometry", this, QRect(0, 0, 1000, 600));
	setGeometry(geometry);
	loadSplitterConfigs();
	loadTreeViewConfigs();
	loadEditorConfigs();
	loadPreviewConfigs();
	loadMenuBarConfigs();
}

void MainWindow::loadSplitterConfigs()
{
	auto state = loadConfig<QByteArray>("position", m_splitter);
	m_splitter->restoreState(state);
}

void MainWindow::loadTreeViewConfigs()
{
	//
}

void MainWindow::loadEditorConfigs()
{
	auto font = loadConfig<QFont>("font", m_editor, m_editor->defaulFont());
	m_editor->setFont(font);
}

void MainWindow::loadPreviewConfigs()
{
	//
}

void MainWindow::loadMenuBarConfigs()
{
	loadConfigPassthrough<QString>("theme", m_editor, [&](QString theme) {

		auto fs_editor_theme = Path::toStdFs(theme);
		m_stylist->style(m_editor, fs_editor_theme);
		m_menuBar->setSelectedEditorTheme(fs_editor_theme);

		}, Path::toQString(m_menuBar->defaultEditorTheme()));

	loadConfigPassthrough<QString>("theme", this, [&](QString theme) {

		auto fs_window_theme = Path::toStdFs(theme);
		m_stylist->style(this, fs_window_theme);
		m_menuBar->setSelectedWindowTheme(fs_window_theme);

		}, Path::toQString(m_menuBar->defaultWindowTheme()));
}

void MainWindow::closeEventConfigs(Qt::WindowStates priorState)
{
	saveConfigPassthrough(priorState, "state", this);
	saveConfigPassthrough(geometry(), "geometry", this);
	saveConfigPassthrough(m_splitter->saveState(), "position", m_splitter);
}
