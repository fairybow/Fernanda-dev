#pragma once

#include "common/Layout.hpp"

#include <QProgressBar>
#include <QTimer>
#include <QWidget>

class Indicator : public QWidget
{
	Q_OBJECT

public:
	inline Indicator(const char* name, QWidget* parent)
		: QWidget(parent)
	{
		m_progressBar->setObjectName(name);
		setAttribute(Qt::WA_TransparentForMouseEvents);
		m_progressBar->setAttribute(Qt::WA_TransparentForMouseEvents);
		Layout::setVBoxLayout(this, m_progressBar);

		m_progressBar->setMaximumHeight(3);
		m_progressBar->setTextVisible(false);
		m_progressBar->setRange(0, 100);
		m_progressBar->hide();

		connect(m_timer, &QTimer::timeout, this, [&] {
			m_progressBar->hide();
			m_progressBar->reset();
			});
	}

private:
	QProgressBar* m_progressBar = new QProgressBar(this);
	QTimer* m_timer = new QTimer(this);
};
