#pragma once

#include "../common/StringTools.hpp"
#include "../common/Utility.hpp"
#include "ToolButton.hpp"

#include <QMainWindow>
#include <QMessageBox>
#include <QMouseEvent>
#include <QString>
#include <QTimer>

class PomodoroTimer : public ToolButton
{
	Q_OBJECT

public:
	PomodoroTimer(const QString& text, QMainWindow* mainWindow,
		QWidget* parent = nullptr, int defaultSecondsCountdown = defaultInterval())
		: ToolButton(text, parent),
		m_text(text),
		m_window(mainWindow),
		m_interval(defaultSecondsCountdown)
	{
		m_timer->setTimerType(Qt::PreciseTimer);
		connect(m_timer, &QTimer::timeout, this, &PomodoroTimer::countdownDisplay);
		connect(this, &PomodoroTimer::toggled, this, &PomodoroTimer::startCountdown);
	}

	static int defaultInterval() { return 1500; }

	void setCountdown(int seconds) { m_interval = qBound(30, seconds, 3600); }

protected:
	virtual void mousePressEvent(QMouseEvent* event) override
	{
		if (event->button() != Qt::RightButton)
			if (pauseOrResumeIfRunning()) return;
		ToolButton::mousePressEvent(event);
	}

private:
	const QString m_text;
	int m_interval;
	int m_countdown = 0;
	QMainWindow* m_window;
	QTimer* m_timer = new QTimer(this);

	void timeUp(QMainWindow* parentWindow)
	{
		QMessageBox popup(parentWindow);
		popup.setWindowTitle(parentWindow->windowTitle());
		popup.setText(
			StringTools::pad("Time's up!", 30));
		auto ok = popup.addButton(QMessageBox::Ok);
		popup.setDefaultButton(ok);
		popup.exec();
	}

	bool isStopping(bool checked)
	{
		if (!checked) {
			setText(m_text);
			m_timer->stop();
			m_countdown = m_interval;
			return true;
		}
		return false;
	}

	bool pauseOrResumeIfRunning()
	{
		if (m_countdown > 0 && m_countdown < m_interval) {
			m_timer->isActive()
				? m_timer->stop()
				: m_timer->start(1000);
			return true;
		}
		return false;
	}

private slots:
	void countdownDisplay()
	{
		setText("  " + m_text + "  " + StringTools::secondsToMinutes(m_countdown, ".") + "  ");
		if (m_countdown < 1) {
			timeUp(m_window);
			setChecked(false);
			return;
		}
		Utility::delayCall(this, [&] { --m_countdown; });
	}

	void startCountdown(bool checked)
	{
		if (isStopping(checked)) return;
		m_countdown = m_interval;
		m_timer->start(1000);
	}
};
