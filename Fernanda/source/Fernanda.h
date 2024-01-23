#pragma once

#include "common/Path.hpp"
#include "window/Window.h"
#include "window/WindowSettings.h"

#include <QDebug>
#include <QList>
#include <QMenu>
#include <QMenuBar>
#include <QObject>

class Fernanda : public QObject
{
	Q_OBJECT

public:
	Fernanda(bool isDev);
	~Fernanda() { qDebug() << __FUNCTION__; }

	Window* newWindow();

private:
	static constexpr char SETTINGS_INI[] = ".fernanda/Settings.ini";

	bool m_isDev;
	const Path& m_iniPath = Path::system(Path::System::Home) / SETTINGS_INI;
	WindowSettings* m_windowSettings = new WindowSettings(m_iniPath, this);
	QList<Window*> m_windows;

	void setupWindow(Window* window);

	void addMenuBar(Window* window);
	QMenu* file(QMenuBar* menuBar, Window* window);
	QMenu* view(QMenuBar* menuBar, Window* window);
	QMenu* help(QMenuBar* menuBar, Window* window);
	QMenu* dev(QMenuBar* menuBar, Window* window);

private slots:
	void onWindowFileDoubleClicked(const Path& path);
	void onWindowClosing();
};
