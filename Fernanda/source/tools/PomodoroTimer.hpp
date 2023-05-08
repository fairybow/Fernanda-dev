#pragma once

#include "Tool.hpp"

#include <QMainWindow>
#include <QMessageBox>
#include <QString>
#include <QTimer>

class PomodoroTimer : public Tool
{
	Q_OBJECT

public:
	PomodoroTimer(const QString& text, QMainWindow* mainWindow, QWidget* parent = nullptr, int defaultSecondsCountdown = 1500)
		: Tool(text, parent), m_text(text), m_window(mainWindow), m_defaultCountdown(defaultSecondsCountdown)
	{
		m_timer->setTimerType(Qt::PreciseTimer);
		connect(m_timer, &QTimer::timeout, this, &PomodoroTimer::countdownDisplay);
		connect(this, &PomodoroTimer::toggled, this, &PomodoroTimer::startCountdown);
	}

	void setCountdown(int seconds) { m_countdown = seconds; }

/*signals:
	int resetCountdown();*/

private:
	const QString m_text;
	const int m_defaultCountdown;
	int m_countdown = m_defaultCountdown;
	QMainWindow* m_window;
	QTimer* m_timer = new QTimer(this);

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

private slots:
	void countdownDisplay()
	{
		setText("  " + m_text + "  " + time(m_countdown) + "  ");
		if (m_countdown < 1) {
			timeUp(m_window);
			setChecked(false);
			return;
		}
		--m_countdown;
	}

	void startCountdown(bool checked)
	{
		if (!checked) {
			setText(m_text);
			m_timer->stop();
			m_countdown = /*emit resetCountdown()*/ m_defaultCountdown;
			return;
		}
		m_timer->start(1000);
	}
};
