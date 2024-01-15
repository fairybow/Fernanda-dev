#include "common/Connect.hpp"
#include "Fernanda.h"

//#include <QByteArray>
#include <QStandardPaths>

Fernanda::Fernanda(bool isDev)
	: m_isDev(isDev)
{
	setup();
}

Window* Fernanda::newWindow()
{
	auto window = new Window;
	m_windows << window;

	setupWindow(window);
	window->show();

	return window;
}

void Fernanda::setup()
{
	connect(m_windowSettings, &WindowSettings::settingChanged, this, &Fernanda::onWindowSettingsSettingChanged);
}

void Fernanda::setupWindow(Window* window)
{
	m_windowSettings->applyAll(window);

	addMenuBar(window);

	connect(window, &Window::treeViewFileDoubleClicked, this, &Fernanda::onWindowFileDoubleClicked);
	connect(window, &Window::closing, this, &Fernanda::onWindowClosing);
}

void Fernanda::addMenuBar(Window* window)
{
	auto bar = new QMenuBar;

	bar->addMenu(file(bar, window));
	bar->addMenu(view(bar, window));
	bar->addMenu(help(bar, window));

	if (m_isDev)
		bar->addMenu(dev(bar, window));

	window->setMenuBar(bar);
}

QMenu* Fernanda::file(QMenuBar* menuBar, Window* window)
{
	auto menu = new QMenu("File", menuBar);

	auto test = new QAction("Print window memory address", menu);
	auto new_window = new QAction("New window", menu);

	menu->addAction(test);
	menu->addAction(new_window);

	connect(test, &QAction::triggered, this, [=] { qDebug() << window; });
	connect(new_window, &QAction::triggered, this, [&] { newWindow(); });

	return menu;
}

QMenu* Fernanda::view(QMenuBar* menuBar, Window* window)
{
	auto menu = new QMenu("View", menuBar);

	auto window_settings = new QAction("Window Settings", menu);
	menu->addAction(window_settings);
	connect(window_settings, &QAction::triggered, this, [&] {
		m_windowSettings->openDialog();
		});

	return menu;
}

QMenu* Fernanda::help(QMenuBar* menuBar, Window* window)
{
	auto menu = new QMenu("Help", menuBar);

	//

	return menu;
}

QMenu* Fernanda::dev(QMenuBar* menuBar, Window* window)
{
	auto menu = new QMenu("Dev", menuBar);

	//

	return menu;
}

void Fernanda::onWindowFileDoubleClicked(const Path& path)
{
	auto sender_window = sender_cast(Window);
	if (!sender_window) return;

	for (auto& window : m_windows)
		if (window->find(path)) {
			window->raise();
			window->activateWindow();

			return;
		}

	sender_window->open(path);
}

void Fernanda::onWindowClosing()
{
	auto window = sender_cast(Window);
	if (!window) return;

	m_windows.removeAll(window);

	delete window;
}

void Fernanda::onWindowSettingsSettingChanged(WindowSettings::Type type)
{
	m_windowSettings->applySetting(m_windows, type);
}
