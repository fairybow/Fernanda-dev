#pragma once

#include "common/Path.hpp"
#include "menus/Menus.h"
#include "Window.h"

#include <QDebug>
#include <QList>
#include <QMenuBar>

class Fernanda : public QObject
{
	Q_OBJECT

public:
	Fernanda(bool isDev);
	~Fernanda() { qDebug() << __FUNCTION__; }

	void newWindow();

private:
	Menus* m_menus = new Menus(this);

	bool m_isDev;
	QList<Window*> m_windows;

	void setupWindow(Window* window);
	QMenuBar* menuBar(Window* window);

private slots:
	void onWindowFileDoubleClicked(const Path& path);
	void onWindowClosing();
};
