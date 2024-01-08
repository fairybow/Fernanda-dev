#include "common/Connect.hpp"
#include "Fernanda.h"

#include <QByteArray>
#include <QStandardPaths>

// TESTING
#include <QTimer>

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
	connect(m_windowSettings, &WindowSettings::settingChecked, this, &Fernanda::onWindowSettingChecked);

	// TESTING
	QTimer::singleShot(0, [&] {

		m_windowSettings->openDialog();

		});
}

void Fernanda::setupWindow(Window* window)
{
	window->setAttribute(Qt::WA_DeleteOnClose);

	connect(window, &Window::treeViewFileDoubleClicked, this, &Fernanda::onWindowFileDoubleClicked);
	connect(window, &Window::closing, this, &Fernanda::onWindowClosing);
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

void Fernanda::onWindowSettingChecked(bool checked, WindowSettings::Type type)
{
	m_windowSettings->applySetting(m_windows, checked, type);
}
