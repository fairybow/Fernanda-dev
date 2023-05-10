#pragma once

#include "common/Layout.hpp"
#include "common/Widget.hpp"

#include <QProgressBar>
#include <QTimer>

class Indicator : public Widget<>
{
	Q_OBJECT

public:
	Indicator(const char* name, QWidget* parent)
		: Widget(name, parent)
	{
		buildProgressBar(name);
		Layout::transpareForMouse({ this, m_progressBar });
		Layout::box(Layout::Line::Vertically, m_progressBar, this);
	}

private:
	QProgressBar* m_progressBar = new QProgressBar(this);
	QTimer* m_timer = new QTimer(this);

	void buildProgressBar(const char* name)
	{
		m_progressBar->setObjectName(name);
		m_progressBar->setMaximumHeight(3);
		m_progressBar->setTextVisible(false);
		m_progressBar->setRange(0, 100);
		m_progressBar->hide();
		connect(m_timer, &QTimer::timeout, this, [&] {
			m_progressBar->hide();
			m_progressBar->reset();
			});
	}
};
