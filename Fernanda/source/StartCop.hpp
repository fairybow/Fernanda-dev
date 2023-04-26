#pragma once

#include <QApplication>
#include <QLocalServer>
#include <QLocalSocket>
#include <QString>
#include <qsystemdetection.h>
#include <QWidget>

#ifdef Q_OS_WINDOWS

#include <Windows.h>
#include <WinUser.h>

#endif

class StartCop : public QObject
{
	Q_OBJECT

public:
	inline StartCop(const QString& name) : m_name(name) {}

	inline bool exists() const
	{
		if (serverExists())
			return true;
		startServer();
		return false;
	}

private:
	const QString m_name;

	inline bool serverExists() const
	{
		QLocalSocket socket;
		socket.connectToServer(m_name);
		auto exists = socket.isOpen();
		socket.close();
		return exists;
	}

	inline void startServer() const
	{
		auto server = new QLocalServer;
		server->setSocketOptions(QLocalServer::WorldAccessOption);
		server->listen(m_name);
		connect(server, &QLocalServer::newConnection, this, &StartCop::focusMainWindow);
	}

private slots:
	inline void focusMainWindow() const
	{
		for (auto& widget : QApplication::allWidgets()) {
			if (widget->objectName() != "MainWindow") continue;
			if (widget->windowState() == Qt::WindowMinimized)
				widget->setWindowState((widget->windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);

#ifdef Q_OS_WINDOWS

			/*auto name = widget->windowTitle().toStdWString();
			auto handle = FindWindow(0, name.c_str());
			SwitchToThisWindow(handle, FALSE);*/

			/*[This function is not intended for general use. It may be altered or unavailable in subsequent versions of Windows.]*/

#endif

			widget->activateWindow();
			break;
		}
	}
};
