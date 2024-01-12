#include "WindowMenu.h"

WindowMenu::WindowMenu(Window* window)
	: QMenuBar(window)
{
	/*for (auto& action : {
		g_meterLinePos,
		g_meterColPos,
		g_meterLineCount,
		g_meterWordCount,
		g_meterCharCount
		})
		action->setCheckable(true);*/

	setup();
}

void WindowMenu::setup()
{
	addMenu(file());
	addMenu(view());
	addMenu(help());

	/*if (m_isDev) {
		addMenu(dev());
	}*/
}

QMenu* WindowMenu::file()
{
	auto menu = new QMenu("File", this);

	auto new_window = new QAction("New window", menu);
	
	menu->addAction(new_window);

	connect(new_window, &QAction::triggered, m_window, [&] {
		//emit newWindowRequested();
		qDebug() << __FUNCTION__;
		});

	return menu;
}

QMenu* WindowMenu::view()
{
	auto menu = new QMenu("View", this);

	/*for (auto& action : {
		g_meterLinePos,
		g_meterColPos,
		g_meterLineCount,
		g_meterWordCount,
		g_meterCharCount
		})
		menu->addAction(action);

	connect(g_meterLinePos, &QAction::triggered, m_window, [&](bool checked) {
		// for all windows, do the thing and write to settings?

		//emit xxxxx(); // Will emit signal from each existing window

		});
	connect(g_meterColPos, &QAction::triggered, m_window, [&](bool checked) {
		//
		});
	connect(g_meterLineCount, &QAction::triggered, m_window, [&](bool checked) {
		//
		});
	connect(g_meterWordCount, &QAction::triggered, m_window, [&](bool checked) {
		//
		});
	connect(g_meterCharCount, &QAction::triggered, m_window, [&](bool checked) {
		//
		});*/

	return menu;
}

QMenu* WindowMenu::help()
{
	auto menu = new QMenu("Help", this);

	auto test = new QAction("Print window memory address", menu);
	connect(test, &QAction::triggered, m_window, [=] { qDebug() << m_window; });
	menu->addAction(test);

	return menu;
}

QMenu* WindowMenu::dev()
{
	auto menu = new QMenu("Dev", this);

	auto test = new QAction("Print window memory address", menu);
	connect(test, &QAction::triggered, m_window, [=] { qDebug() << m_window; });
	menu->addAction(test);

	return menu;
}
