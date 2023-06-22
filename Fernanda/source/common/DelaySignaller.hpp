#pragma once

#include <QTimer>

class DelaySignaller : public QObject
{
	Q_OBJECT

public:
	DelaySignaller(QObject* parent, int millisecondThreshhold = 10000)
		: QObject(parent), m_threshhold(millisecondThreshhold)
	{
		m_delay->setSingleShot(true);
		connect(m_delay, &QTimer::timeout, this, [&] {
			emit signal();
			});
	}

	void delayedEmit(int length)
	{
		(length < m_threshhold)
			? m_delay->setInterval(0)
			: m_delay->setInterval((length / m_threshhold) * 1000);
		m_delay->start();
	}

signals:
	void signal();

private:
	const int m_threshhold;
	QTimer* m_delay = new QTimer(this);
};
