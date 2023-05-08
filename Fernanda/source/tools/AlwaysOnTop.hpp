#pragma once

#include "Tool.hpp"

#include <QMainWindow>
#include <QString>

class AlwaysOnTop : public Tool
{
	Q_OBJECT

public:
	AlwaysOnTop(const QString& text, QMainWindow* mainWindow, QWidget* parent = nullptr)
		: Tool(text, parent), m_window(mainWindow)
	{
		connect(this, &AlwaysOnTop::toggled, this, &AlwaysOnTop::alwaysOnTop);
	}

private:
	QMainWindow* m_window;

private slots:
	void alwaysOnTop()
	{
		isChecked()
			? m_window->setWindowFlags(m_window->windowFlags() | Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint)
			: m_window->setWindowFlags(m_window->windowFlags() ^ (Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint));
		m_window->show();
	}
};
