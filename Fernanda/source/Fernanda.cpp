#include "common/Connect.hpp"
#include "Fernanda.h"

Fernanda::Fernanda(bool isDev)
	: m_isDev(isDev)
{}

void Fernanda::newWindow()
{
	auto window = new Window;
	m_windows << window;

	setupWindow(window);

	window->show();
}

void Fernanda::setupWindow(Window* window)
{
	window->setAttribute(Qt::WA_DeleteOnClose);

	auto menu_bar = menuBar(window);
	window->setMenuBar(menu_bar);

	connect(window, &Window::treeViewFileDoubleClicked, this, &Fernanda::onWindowFileDoubleClicked);
	connect(window, &Window::closing, this, &Fernanda::onWindowClosing);
}

QMenuBar* Fernanda::menuBar(Window* window)
{
	auto menu_bar = new QMenuBar;

	auto file = m_menus->file(menu_bar, window);
	menu_bar->addMenu(file);

	auto view = m_menus->view(menu_bar, window);
	menu_bar->addMenu(view);

	auto help = m_menus->help(menu_bar, window);
	menu_bar->addMenu(help);

	if (m_isDev) {
		auto dev = m_menus->dev(menu_bar, window);
		menu_bar->addMenu(dev);
	}

	return menu_bar;
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
}
