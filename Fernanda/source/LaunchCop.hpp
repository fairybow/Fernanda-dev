#pragma once

#include <QApplication>
#include <QByteArray>
#include <QLocalServer>
#include <QLocalSocket>
#include <QMainWindow>
#include <QString>

#ifdef Q_OS_WINDOWS

#include <Windows.h>
#include <WinUser.h>

#endif

#include <algorithm>

class LaunchCop : public QObject
{
	Q_OBJECT

public:
	LaunchCop(const QString& lockString, const QString& mainWindowObjectName = "MainWindow",
		bool forceFocus = false)
		: m_serverName(lockString),
		m_windowName(mainWindowObjectName),
		m_forceFocus(forceFocus) {}

	bool isRunning() const
	{
		qDebug() << __FUNCTION__;

		if (serverExists())
			return true;

		startServer();
		return false;
	}

signals:
	void launchedAgain() const;

private:
	const QString m_serverName;
	const QString m_windowName;
	const bool m_forceFocus;

	bool serverExists() const
	{
		QLocalSocket socket;
		socket.connectToServer(m_serverName);
		auto exists = socket.isOpen();
		socket.close();
		return exists;
	}

	void startServer() const
	{
		qDebug() << __FUNCTION__;
		auto server = new QLocalServer;
		server->setSocketOptions(QLocalServer::WorldAccessOption);
		server->listen(m_serverName);

		qDebug() << connect(server, &QLocalServer::newConnection, this, &LaunchCop::focusMainWindow);
		qDebug() << connect(server, &QLocalServer::newConnection, this, [&] { emit launchedAgain(); });
	}

private slots:
	void focusMainWindow() const
	{
		qDebug() << __FUNCTION__;

		auto top_widgets = QApplication::topLevelWidgets();
		auto it = std::find_if(top_widgets.begin(), top_widgets.end(),
			[&](QWidget* widget) { return widget->objectName() == m_windowName; });
		if (it == top_widgets.end()) return;
		auto main_window = *it;
		if (main_window->windowState() == Qt::WindowMinimized)
			main_window->setWindowState((main_window->windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);

#ifdef Q_OS_WINDOWS

		if (m_forceFocus) {
			auto name = main_window->windowTitle().toStdWString();
			auto handle = FindWindow(0, name.c_str());
			SwitchToThisWindow(handle, FALSE);
			// [This function is not intended for general use. It may be altered or unavailable in subsequent versions of Windows.]
		}

#endif

		main_window->activateWindow();
	}
};
