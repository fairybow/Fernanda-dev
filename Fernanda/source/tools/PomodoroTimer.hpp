#pragma once

#include "../common/Event.hpp"
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
	PomodoroTimer(const QString& text, QMainWindow* mainWindow, QWidget* parent = nullptr, int defaultSecondsCountdown = 1500)
		: ToolButton(text, parent), m_text(text), m_window(mainWindow), m_backupCountdown(defaultSecondsCountdown)
	{
		m_timer->setTimerType(Qt::PreciseTimer);
		connect(m_timer, &QTimer::timeout, this, &PomodoroTimer::countdownDisplay);
		connect(this, &PomodoroTimer::toggled, this, &PomodoroTimer::startCountdown);
	}

	void setCountdown(int seconds) { m_countdown = qBound(30, seconds, 3600); }

signals:
	int getCurrentDefault();

protected:
	virtual void mousePressEvent(QMouseEvent* event) override
	{
		if (event->button() != Qt::RightButton)
			if (pauseOrResumeIfRunning()) return;
		ToolButton::mousePressEvent(event);
	}

private:
	const QString m_text;
	const int m_backupCountdown;
	int m_countdown = 0;
	QMainWindow* m_window;
	QTimer* m_timer = new QTimer(this);

	int currentDefaultCountdown()
	{
		auto default_value = m_backupCountdown;
		if (emit getCurrentDefault() > 0)
			default_value = emit getCurrentDefault();
		return default_value;
	}

	const QString time(int seconds)
	{
		auto time_seconds = seconds % 60;
		QString seconds_string;
		(time_seconds <= 9)
			? seconds_string = "0" + QString::number(time_seconds)
			: seconds_string = QString::number(time_seconds);
		return QString::number((seconds / 60) % 60) + "." + seconds_string;
	}

	void timeUp(QMainWindow* parentWindow)
	{
		QMessageBox popup(parentWindow);
		popup.setWindowTitle(parentWindow->windowTitle());
		popup.setText("Time's up!          "); // -_-
		auto ok = popup.addButton(QMessageBox::Ok);
		popup.setDefaultButton(ok);
		popup.exec();
	}

	bool isStopping(bool checked)
	{
		if (!checked) {
			setText(m_text);
			m_timer->stop();
			m_countdown = currentDefaultCountdown();
			return true;
		}
		return false;
	}

	bool pauseOrResumeIfRunning()
	{
		if (m_countdown > 0 && m_countdown < currentDefaultCountdown()) {
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
		setText("  " + m_text + "  " + time(m_countdown) + "  ");
		if (m_countdown < 1) {
			timeUp(m_window);
			setChecked(false);
			return;
		}
		Event::delayCall(this, [&] { --m_countdown; });
	}

	void startCountdown(bool checked)
	{
		if (isStopping(checked)) return;
		m_countdown = currentDefaultCountdown();
		m_timer->start(1000);
	}
};
