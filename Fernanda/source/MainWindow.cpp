#include "MainWindow.h"

MainWindow::MainWindow(const char* name, bool isDev, StdFsPath file, QWidget* parent)
	: Widget(name, parent), m_isDev(isDev)
{
	setupWidgets();
	connections();
	loadConfigs();
	newTab();
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

void MainWindow::showEvent(QShowEvent* event)
{
	QMainWindow::showEvent(event);
	if (m_isInitialized || event->spontaneous()) return;
	m_indicator->pastel(1500);
	m_isInitialized = true;
}

void MainWindow::setupWidgets()
{
	setMenuBar(m_menuBar);
	setStatusBar(m_statusBar);
	m_statusBar->addWidgets({ m_meter }, { m_pomodoroTimer, m_stayAwake, m_alwaysOnTop });
	m_menuBar->makeSubmenus();
	auto layout = Layout::box(Layout::Line::Vertically, nullptr, nullptr, { 4, 0, 4, 0 });
	layout->addWidget(m_tabBar);
	layout->addLayout(Layout::stack({ m_editor, m_indicator }));
	auto container = Layout::container(layout);
	setCentralWidget(container);
}

void MainWindow::connections()
{
	documentConnections();
	tabBarConnections();
	editorConnections();
	meterConnections();
	//previewConnections();
	menuBarConnections();
	menuBarStyleConfigConnections();
	menuBarEditorConfigConnections();
	menuBarMeterConfigConnections();
	menuBarToolConfigConnections();
	menuBarMiscConfigConnections();
}

void MainWindow::documentConnections()
{
	connect(m_document, &Document::askSetText, this, [&] {
		m_document->setText(m_editor->toPlainText());
		});
	connect(m_document, &Document::editedStateChanged, m_tabBar, &TabBar::updateEditedState);

	auto timer = new QTimer(this);
	timer->setSingleShot(true);
	timer->setInterval(1000);

	connect(m_editor, &Editor::textChanged, this, [&, timer] {
		timer->start();
		});
	connect(timer, &QTimer::timeout, this, [&] {
		auto text = m_editor->toPlainText();
		m_document->affirmEditedState(text);
		qDebug() << m_document->editedState();
		});
}

void MainWindow::tabBarConnections()
{
	connect(m_tabBar, &TabBar::currentChanged, this, &MainWindow::openTab);
	connect(m_tabBar, &TabBar::askNew, this, &MainWindow::newTab);
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
	connect(m_menuBar, &MenuBar::getUserFont, this, [&] {
		return loadConfig<QFont>(Ini::EDITOR_FONT, m_editor, m_editor->defaulFont());
		});
	connect(m_menuBar, &MenuBar::askOpenNewFile, this, [&](StdFsPath path) {
		menuBarOpenNewFile(path);
		});
	connect(m_menuBar, &MenuBar::askOpenFile, this, [&](StdFsPath path) {
		menuBarOpenFile(path);
		});
}

void MainWindow::menuBarStyleConfigConnections()
{
	connect(m_menuBar, &MenuBar::askToggleEditorTheme, this, [&](bool state) {
		saveConfigPassthrough(
			state, Ini::HAS_EDITOR_THEME, m_editor, [&] {
				m_stylist->setThemeEnabled(m_editor, state);
			});
		});

	connect(m_menuBar, &MenuBar::askToggleWindowTheme, this, [&](bool state) {
		saveConfigPassthrough(
			state, Ini::HAS_WINDOW_THEME, this, [&] {
				m_stylist->setThemeEnabled(this, state);
			});
		});

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
}

void MainWindow::menuBarEditorConfigConnections()
{
	connect(m_menuBar, &MenuBar::askChangeFont, this, [&](const QFont& font) {
		saveConfigPassthrough(
			font, Ini::EDITOR_FONT, m_editor, [&] {
				m_editor->setFont(font);
			});
		});

	connect(m_menuBar, &MenuBar::askSetTabStop, this, [&](int pixels) {
		saveConfigPassthrough(
			pixels, Ini::EDITOR_TAB_STOP, m_editor, [&] {
				m_editor->setTabStopDistance(pixels);
			});
		});

	connect(m_menuBar, &MenuBar::askSetWrapMode, this, [&](const QString& mode) {
		saveConfigPassthrough(
			mode, Ini::EDITOR_WRAP_MODE, m_editor, [&] {
				m_editor->setWrapMode(mode);
			});
		});

	connect(m_menuBar, &MenuBar::askToggleLineHighlight, this, [&](bool state) {
		saveConfigPassthrough(
			state, Ini::EDITOR_LINE_HIHGLIGHT, m_editor, [&] {
				m_editor->setHasLineHighlight(state);
			});
		});

	connect(m_menuBar, &MenuBar::askToggleLineNumbers, this, [&](bool state) {
		saveConfigPassthrough(
			state, Ini::EDITOR_LINE_NUMBER_AREA, m_editor, [&] {
				m_editor->setHasLineNumberArea(state);
			});
		});

	connect(m_menuBar, &MenuBar::askToggleShadow, this, [&](bool state) {
		saveConfigPassthrough(
			state, Ini::EDITOR_SHADOW, m_editor, [&] {
				m_editor->setHasShadow(state);
			});
		});

	connect(m_menuBar, &MenuBar::askToggleBlink, this, [&](bool state) {
		saveConfigPassthrough(
			state, Ini::CURSOR_BLINK, m_editor, [&] {
				m_editor->setHasCursorBlink(state);
			});
		});

	connect(m_menuBar, &MenuBar::askToggleBlock, this, [&](bool state) {
		saveConfigPassthrough(
			state, Ini::CURSOR_BLOCK, m_editor, [&] {
				m_editor->setHasCursorBlock(state);
			});
		});

	connect(m_menuBar, &MenuBar::askToggleCenterOnScroll, this, [&](bool state) {
		saveConfigPassthrough(
			state, Ini::CURSOR_CENTER_ON_SCROLL, m_editor, [&] {
				m_editor->setCenterOnScroll(state);
			});
		});

	connect(m_menuBar, &MenuBar::askToggleEnsureVisible, this, [&](bool state) {
		saveConfigPassthrough(
			state, Ini::CURSOR_ENSURE_VISIBLE, m_editor, [&] {
				m_editor->setHasCursorEnsureVisible(state);
			});
		});

	connect(m_menuBar, &MenuBar::askToggleTypewriter, this, [&](bool state) {
		saveConfigPassthrough(
			state, Ini::CURSOR_TYPEWRITER, m_editor, [&] {
				m_editor->setHasCursorTypewriter(state);
			});
		});
}

void MainWindow::menuBarMeterConfigConnections()
{
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
}

void MainWindow::menuBarToolConfigConnections()
{
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

void MainWindow::menuBarMiscConfigConnections()
{
	// to-do:
	//void askSetPreviewType(const QString& type);

	connect(m_menuBar, &MenuBar::askToggleIndicator, this, [&](bool state) {
		saveConfigPassthrough(
			state, Ini::IDICATOR, m_indicator, [&] {
				m_indicator->setVisible(state);
			});
		});

	connect(m_menuBar, &MenuBar::askSetIndicatorAlignment, this, [&](const QString& alignment) {
		saveConfigPassthrough(
			alignment, Ini::IDICATOR_ALIGNMENT, m_indicator, [&] {
				m_indicator->setAlignment(alignment);
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
	loadMenuBarStyleConfigs();
	loadMenuBarEditorConfigs();
	loadMenuBarMeterConfigs();
	loadMenuBarToolConfigs();
	loadMenuBarMiscConfigs();
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

void MainWindow::loadMenuBarStyleConfigs()
{
	loadConfigPassthrough<bool>(Ini::HAS_EDITOR_THEME, m_editor, [&](bool state) {
		m_stylist->setThemeEnabled(m_editor, state);
		m_menuBar->setCheckBoxEditorTheme(state);
		}, true);

	loadConfigPassthrough<bool>(Ini::HAS_WINDOW_THEME, this, [&](bool state) {
		m_stylist->setThemeEnabled(this, state);
		m_menuBar->setCheckBoxWindowTheme(state);
		}, true);

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
}

void MainWindow::loadMenuBarEditorConfigs()
{
	loadConfigPassthrough<int>(Ini::EDITOR_TAB_STOP, m_editor, [&](int pixels) {
		m_editor->setTabStopDistance(pixels);
		m_menuBar->setSelectedTabStop(pixels);
		}, m_editor->defaulTabStop());

	loadConfigPassthrough<QString>(Ini::EDITOR_WRAP_MODE, m_editor, [&](QString mode) {
		m_editor->setWrapMode(mode);
		m_menuBar->setSelectedWrapMode(mode);
		}, m_editor->defaultWrap());

	loadConfigPassthrough<bool>(Ini::EDITOR_LINE_HIHGLIGHT, m_editor, [&](bool state) {
		m_editor->setHasLineHighlight(state);
		m_menuBar->setCheckBoxLineHighlight(state);
		}, true);

	loadConfigPassthrough<bool>(Ini::EDITOR_LINE_NUMBER_AREA, m_editor, [&](bool state) {
		m_editor->setHasLineNumberArea(state);
		m_menuBar->setCheckBoxLineNumbers(state);
		}, true);

	loadConfigPassthrough<bool>(Ini::EDITOR_SHADOW, m_editor, [&](bool state) {
		m_editor->setHasShadow(state);
		m_menuBar->setCheckBoxShadow(state);
		}, false);

	loadConfigPassthrough<bool>(Ini::CURSOR_BLINK, m_editor, [&](bool state) {
		m_editor->setHasCursorBlink(state);
		m_menuBar->setCheckBoxBlink(state);
		}, true);

	loadConfigPassthrough<bool>(Ini::CURSOR_BLOCK, m_editor, [&](bool state) {
		m_editor->setHasCursorBlock(state);
		m_menuBar->setCheckBoxBlock(state);
		}, true);

	loadConfigPassthrough<bool>(Ini::CURSOR_CENTER_ON_SCROLL, m_editor, [&](bool state) {
		m_editor->setCenterOnScroll(state);
		m_menuBar->setCheckBoxCenterOnScroll(state);
		}, false);

	loadConfigPassthrough<bool>(Ini::CURSOR_ENSURE_VISIBLE, m_editor, [&](bool state) {
		m_editor->setHasCursorEnsureVisible(state);
		m_menuBar->setCheckBoxEnsureVisible(state);
		}, true);

	loadConfigPassthrough<bool>(Ini::CURSOR_TYPEWRITER, m_editor, [&](bool state) {
		m_editor->setHasCursorTypewriter(state);
		m_menuBar->setCheckBoxTypewriter(state);
		}, false);
}

void MainWindow::loadMenuBarMeterConfigs()
{
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
}

void MainWindow::loadMenuBarToolConfigs()
{
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

void MainWindow::loadMenuBarMiscConfigs()
{
	loadConfigPassthrough<bool>(Ini::IDICATOR, m_indicator, [&](bool state) {
		m_indicator->setVisible(state);
		m_menuBar->setCheckBoxIndicator(state);
		}, true);

	loadConfigPassthrough<QString>(Ini::IDICATOR_ALIGNMENT, m_indicator, [&](const QString& alignment) {
		m_indicator->setAlignment(alignment);
		m_menuBar->setSelectedIndicatorAlignment(alignment);
		}, QString("Top"));

	//void askSetPreviewType(const QString& type);
}

void MainWindow::closeEventConfigs(Qt::WindowStates priorState)
{
	saveConfigPassthrough(priorState, Ini::WINDOW_STATE, this);
	saveConfigPassthrough(geometry(), Ini::WINDOW_GEOMETRY, this);
}

void MainWindow::menuBarOpenFile(StdFsPath path, bool writeNew)
{
	if (path.empty()) {
		m_indicator->red();
		return;
	}
	if (writeNew)
		m_document->writeEmptyFile(path);
	auto text = m_document->setCurrent(path);
	m_editor->setPlainText(text);
	m_tabBar->serve(m_document->currentId(), path);
}

void MainWindow::openTab(int index)
{
	auto extant_id = m_tabBar->id(index);
	auto document_text = m_document->setCurrent(extant_id);
	m_editor->setPlainText(document_text);
	// m_editor-> restore cursor by id
}

void MainWindow::newTab()
{
	auto new_id = m_document->createEmpty();
	m_document->setCurrent(new_id);
	m_editor->clear();
	m_tabBar->serve(new_id);
}
