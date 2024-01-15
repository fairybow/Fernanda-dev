#pragma once

#include <QDebug>
#include <QLocalServer>
#include <QLocalSocket>
#include <QObject>
#include <QString>
#include <QTimer>

class StartCop : public QObject
{
	Q_OBJECT

public:
	StartCop(const QString& lockString)
		: m_serverName(lockString)
	{
		setup();
	}

	~StartCop()
	{
		qDebug() << __FUNCTION__;
	}

	bool isRunning()
	{
		if (serverExists())
			return true;

		startServer();

		return false;
	}

private:
	const QString m_serverName;
	bool m_hasSignalChoke = false;
	QTimer m_signalChokeCountdown;

	bool serverExists() const
	{
		QLocalSocket socket;
		socket.connectToServer(m_serverName);

		auto exists = socket.isOpen();
		socket.close();

		return exists;
	}

	void setup()
	{
		m_signalChokeCountdown.setSingleShot(true);

		connect(&m_signalChokeCountdown, &QTimer::timeout, this, [&] { m_hasSignalChoke = false; });
	}

	void startServer()
	{
		auto server = new QLocalServer;
		server->setSocketOptions(QLocalServer::WorldAccessOption);
		server->listen(m_serverName);

		connect(server, &QLocalServer::newConnection, this, [&] { notify(); });
	}

	void notify()
	{
		if (m_hasSignalChoke) return;

		emit launched();

		m_hasSignalChoke = true;
		m_signalChokeCountdown.start(1000);
	}

signals:
	void launched();
};
