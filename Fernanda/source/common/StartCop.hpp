#pragma once

#include <QApplication>
#include <QLocalServer>
#include <QLocalSocket>
#include <QMainWindow>
#include <QString>
#include <qsystemdetection.h>
#include <QWidget>

#ifdef Q_OS_WINDOWS

#include <Windows.h>
#include <WinUser.h>

#endif

#include <algorithm>

class StartCop : public QObject
{
	Q_OBJECT

public:
	inline StartCop(const QString& lockString, const QString& mainWindowObjectName = "MainWindow")
		: m_lockString(lockString), m_windowName(mainWindowObjectName) {}

	inline bool exists() const
	{
		if (serverExists())
			return true;
		startServer();
		return false;
	}

private:
	const QString m_lockString;
	const QString m_windowName;

	inline bool serverExists() const
	{
		QLocalSocket socket;
		socket.connectToServer(m_lockString);
		auto exists = socket.isOpen();
		socket.close();
		return exists;
	}

	inline void startServer() const
	{
		auto server = new QLocalServer;
		server->setSocketOptions(QLocalServer::WorldAccessOption);
		server->listen(m_lockString);
		connect(server, &QLocalServer::newConnection, this, &StartCop::focusMainWindow);
	}

private slots:
	inline void focusMainWindow() const
	{
		auto top_widgets = QApplication::topLevelWidgets();
		auto it = std::find_if(top_widgets.begin(), top_widgets.end(),
			[&](QWidget* widget) { return widget->objectName() == m_windowName; });
		if (it == top_widgets.end()) return;
		auto main_window = *it;
		if (main_window->windowState() == Qt::WindowMinimized)
			main_window->setWindowState((main_window->windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);

#ifdef Q_OS_WINDOWS

		/*auto name = main_window->windowTitle().toStdWString();
		auto handle = FindWindow(0, name.c_str());
		SwitchToThisWindow(handle, FALSE);*/

		/*[This function is not intended for general use. It may be altered or unavailable in subsequent versions of Windows.]*/

#endif

		main_window->activateWindow();
	}
};
