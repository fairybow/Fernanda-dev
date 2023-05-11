#include "MainWindow.h"

MainWindow::MainWindow(const char* name, bool isDev, StdFsPath file, QWidget* parent)
	: Widget(name, parent), m_isDev(isDev)
{
	setupWidgets();
	connections();
	loadConfigs();

	// testing

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

	// testing

	//m_tabBar->addTab("Untitled 1");
	//m_tabBar->addTab("Untitled 2");
	//m_tabBar->addTab("Untitled 3");

	//

	Layout::setCentralWidgets(this,
		{ /*m_tabBar, m_indicator,*/ m_editor}, {4, 0, 4, 0}, Layout::Line::Vertically);
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

void MainWindow::menuBarConnections() // things MenuBar needs in order to function
{
	connect(m_menuBar, &MenuBar::getUserDataPath, this, [&] {
		return m_user->dataFolder();
		});
	connect(m_menuBar, &MenuBar::getUserFont, this, [&] {
		return loadConfig<QFont>(Ini::EDITOR_FONT, m_editor, m_editor->defaulFont());
		});
}

void MainWindow::menuBarConfigConnections() // things that *other things* need in order to function, controlled via menu
{
	// further subdivide menuBarConfigConnection fuctions

	connect(m_menuBar, &MenuBar::askStyleEditor, this, [&](StdFsPath path) {
		saveConfigPassthrough(
			Path::toQString(path), Ini::EDITOR_THEME, m_editor, [&] {
				m_stylist->style(m_editor, path);
			});
		});

	connect(m_menuBar, &MenuBar::askStyleWindow, this, [&](StdFsPath path) {
		saveConfigPassthrough(
			Path::toQString(path), Ini::WINDOW_THEME, this, [&] {
				m_stylist->style(this, path);
			});
		});

	connect(m_menuBar, &MenuBar::askChangeFont, this, [&](const QFont& font) {
		saveConfigPassthrough(
			font, Ini::EDITOR_FONT, m_editor, [&] {
				m_editor->setFont(font);
			});
		});

	connect(m_menuBar, &MenuBar::askSetTabStop, this, [&](int pixels) {
		saveConfigPassthrough(
			pixels, Ini::EDITOR_TAB_STOPS, m_editor, [&] {
				m_editor->setTabStopDistance(pixels);
			});
		});

	// to-do:
	//void askSetWrapMode(const QString& mode);
	//void askSetIndicatorPosition(const QString& position);
	//void askSetPreviewType(const QString& type);
	/*bool m_hasCursorBlink = true;
	bool m_hasCursorBlock = true;
	bool m_hasCursorCenterOnScroll = true;
	bool m_hasCursorEnsureVisible = true;
	bool m_hasCursorTypewriter = true;
	bool m_hasLineHighlight = true;
	bool m_hasLineNumberArea = true;
	bool m_hasShadow = true;*/
	
	// toggle entire meter

	connect(m_menuBar, &MenuBar::askToggleLinePosition, this, [&](bool state) {
		saveConfigPassthrough(
			state, Ini::METER_LINE_POS, m_meter, [&] {
				m_meter->setHasLinePosition(state);
			});
		});

	connect(m_menuBar, &MenuBar::askToggleColumnPosition, this, [&](bool state) {
		saveConfigPassthrough(
			state, Ini::METER_COL_POS, m_meter, [&] {
				m_meter->setHasColumnPosition(state);
			});
		});

	connect(m_menuBar, &MenuBar::askToggleLineCount, this, [&](bool state) {
		saveConfigPassthrough(
			state, Ini::METER_LINE_COUNT, m_meter, [&] {
				m_meter->setHasLineCount(state);
			});
		});

	connect(m_menuBar, &MenuBar::askToggleWordCount, this, [&](bool state) {
		saveConfigPassthrough(
			state, Ini::METER_WORD_COUNT, m_meter, [&] {
				m_meter->setHasWordCount(state);
			});
		});

	connect(m_menuBar, &MenuBar::askToggleCharacterCount, this, [&](bool state) {
		saveConfigPassthrough(
			state, Ini::METER_CHAR_COUNT, m_meter, [&] {
				m_meter->setHasCharacterCount(state);
			});
		});

	// toggle all tools

	connect(m_menuBar, &MenuBar::askTogglePomodoroTimer, this, [&](bool state) {
		saveConfigPassthrough(
			state, Ini::TOOL_POMODORO, m_pomodoroTimer, [&] {
				m_pomodoroTimer->setVisible(state);
			});
		});

	connect(m_menuBar, &MenuBar::askToggleStayAwake, this, [&](bool state) {
		saveConfigPassthrough(
			state, Ini::TOOL_STAY_AWAKE, m_stayAwake, [&] {
				m_stayAwake->setVisible(state);
			});
		});

	connect(m_menuBar, &MenuBar::askToggleAlwaysOnTop, this, [&](bool state) {
		saveConfigPassthrough(
			state, Ini::TOOL_ALWAYS_ON_TOP, m_alwaysOnTop, [&] {
				m_alwaysOnTop->setVisible(state);
			});
		});

	connect(m_menuBar, &MenuBar::askSetPomodoroTime, this, [&](int timeInSeconds) {
		saveConfigPassthrough(
			timeInSeconds, Ini::TOOL_POMO_INTERVAL, m_pomodoroTimer, [&] {
				m_pomodoroTimer->setCountdown(timeInSeconds);
			});
		});
}

void MainWindow::loadConfigs()
{
	//auto state = loadConfig(WINDOW_STATE, this, Qt::WindowState::WindowNoState);
	//setWindowState(state); // behaves strangely, Windows issue I think
	auto geometry = loadConfig(Ini::WINDOW_GEOMETRY, this, QRect(0, 0, 1000, 600));
	setGeometry(geometry);
	loadEditorConfigs();
	//loadPreviewConfigs();
	loadMenuBarConfigs();
}

void MainWindow::loadEditorConfigs()
{
	auto font = loadConfig<QFont>(Ini::EDITOR_FONT, m_editor, m_editor->defaulFont());
	m_editor->setFont(font);
}

/*void MainWindow::loadPreviewConfigs()
{
	//
}*/

void MainWindow::loadMenuBarConfigs()
{
	loadConfigPassthrough<QString>(Ini::EDITOR_THEME, m_editor, [&](QString theme) {
		auto fs_editor_theme = Path::toStdFs(theme);
		m_stylist->style(m_editor, fs_editor_theme);
		m_menuBar->setSelectedEditorTheme(fs_editor_theme);
		}, Path::toQString(m_menuBar->defaultEditorTheme()));

	loadConfigPassthrough<QString>(Ini::WINDOW_THEME, this, [&](QString theme) {
		auto fs_window_theme = Path::toStdFs(theme);
		m_stylist->style(this, fs_window_theme);
		m_menuBar->setSelectedWindowTheme(fs_window_theme);
		}, Path::toQString(m_menuBar->defaultWindowTheme()));

	loadConfigPassthrough<int>(Ini::EDITOR_TAB_STOPS, m_editor, [&](int pixels) {
		m_editor->setTabStopDistance(pixels);
		m_menuBar->setSelectedTabStop(pixels);
		}, m_editor->defaulTabStop());

	//void askSetWrapMode(const QString& mode);
	//void askSetIndicatorPosition(const QString& position);
	//void askSetPreviewType(const QString& type);

	// load entire meter box

	loadConfigPassthrough<bool>(Ini::METER_LINE_POS, m_meter, [&](bool state) {
		m_meter->setHasLinePosition(state);
		m_menuBar->setCheckBoxLinePosition(state);
		}, true);

	loadConfigPassthrough<bool>(Ini::METER_COL_POS, m_meter, [&](bool state) {
		m_meter->setHasColumnPosition(state);
		m_menuBar->setCheckBoxColumnPosition(state);
		}, true);

	loadConfigPassthrough<bool>(Ini::METER_LINE_COUNT, m_meter, [&](bool state) {
		m_meter->setHasLineCount(state);
		m_menuBar->setCheckBoxLineCount(state);
		}, false);

	loadConfigPassthrough<bool>(Ini::METER_WORD_COUNT, m_meter, [&](bool state) {
		m_meter->setHasWordCount(state);
		m_menuBar->setCheckBoxWordCount(state);
		}, true);

	loadConfigPassthrough<bool>(Ini::METER_CHAR_COUNT, m_meter, [&](bool state) {
		m_meter->setHasCharacterCount(state);
		m_menuBar->setCheckBoxCharacterCount(state);
		}, false);

	// load entire tools box

	loadConfigPassthrough<bool>(Ini::TOOL_POMODORO, m_pomodoroTimer, [&](bool state) {
		m_pomodoroTimer->setVisible(state);
		m_menuBar->setCheckBoxPomodoroTimer(state);
		}, false);

	loadConfigPassthrough<bool>(Ini::TOOL_STAY_AWAKE, m_stayAwake, [&](bool state) {
		m_stayAwake->setVisible(state);
		m_menuBar->setCheckBoxStayAwake(state);
		}, false);

	loadConfigPassthrough<bool>(Ini::TOOL_ALWAYS_ON_TOP, m_alwaysOnTop, [&](bool state) {
		m_alwaysOnTop->setVisible(state);
		m_menuBar->setCheckBoxAlwaysOnTop(state);
		}, false);

	loadConfigPassthrough<int>(Ini::TOOL_POMO_INTERVAL, m_pomodoroTimer, [&](int timeInSeconds) {
		m_pomodoroTimer->setCountdown(timeInSeconds);
		m_menuBar->setSelectedPomodoroTime(timeInSeconds);
		}, m_pomodoroTimer->defaultInterval());
}

void MainWindow::closeEventConfigs(Qt::WindowStates priorState)
{
	saveConfigPassthrough(priorState, Ini::WINDOW_STATE, this);
	saveConfigPassthrough(geometry(), Ini::WINDOW_GEOMETRY, this);
}
