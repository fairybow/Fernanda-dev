#include "MainWindow.h"

MainWindow::MainWindow(const char* name, bool isDev, StdFsPath file, QWidget* parent)
	: Widget(name, parent), m_isDev(isDev)
{
	setupWidgets();
	connections();
	loadConfigs();

	// testing

	// if opened w/o file, will still have overlay

	m_editor->setFocus();
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
	m_statusBar->addWidgets({ m_meter }, { m_pomodoroTimer, m_stayAwake, m_alwaysOnTop });
	m_menuBar->makeSubmenus();
	Layout::setCentralWidget(this, m_editor, { 4, 0, 4, 0 });
}

void MainWindow::connections()
{
	editorConnections();
	meterConnections();
	//previewConnections();
	menuBarConnections();
	menuBarConfigConnections();
}

void MainWindow::editorConnections()
{
	//
}

void MainWindow::meterConnections()
{
	connect(m_meter, &Meter::getCountsData, this, [&](bool isSelection) {
		if (isSelection)
			return Meter::Counts{ m_editor->selectedText(), m_editor->selectedLineCount() };
		return Meter::Counts{ m_editor->toPlainText(), m_editor->blockCount() };
		});
	connect(m_meter, &Meter::getPositionsData, this, [&] {
		return Meter::Positions{ m_editor->cursorBlockNumber(), m_editor->cursorPositionInBlock() };
		});
	connect(m_meter, &Meter::editorFocusReturn, m_editor, [&] {
		m_editor->setFocus();
		});
	connect(m_editor, &Editor::selectionChanged, this, [&] {
			m_editor->hasSelection()
				? m_meter->trigger(Meter::Type::Selection, true)
				: m_meter->trigger(Meter::Type::Counts, true);
		});
	connect(m_editor, &Editor::textChanged, m_meter, [&] {
		m_meter->trigger(Meter::Type::Counts);
		});
	connect(m_editor, &Editor::cursorPositionChanged, m_meter, [&] {
		m_meter->trigger(Meter::Type::Positions);
		});
}

/*void MainWindow::previewConnections()
{
	//
}*/

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
	loadEditorConfigs();
	//loadPreviewConfigs();
	loadMenuBarConfigs();
}

void MainWindow::loadEditorConfigs()
{
	auto font = loadConfig<QFont>("font", m_editor, m_editor->defaulFont());
	m_editor->setFont(font);
}

/*void MainWindow::loadPreviewConfigs()
{
	//
}*/

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
}
