#pragma once

#include "common/Path.hpp"
#include "menus/Menus.h"
#include "settings/IniWriter.hpp"
#include "Window.h"

#include <QDebug>
#include <QList>
#include <QMenuBar>

// Window can emit a signal to save its size
// Fernanda can directly save Window count
// On start, run the move windows to screen function here for each window

class Fernanda : public QObject
{
	Q_OBJECT

public:
	Fernanda(bool isDev);
	~Fernanda() { qDebug() << __FUNCTION__; }

	void newWindow();

private:
	static constexpr char SETTINGS_INI[] = ".fernanda/Settings.ini";

	Menus* m_menus = new Menus(this); // Move to Window probably
	IniWriter* m_iniWriter = nullptr; // Could pass this to a newly made Window to correctly set MenuBar
	IniWriter* m_sessionWriter = nullptr;

	bool m_isDev;
	QList<Window*> m_windows;

	void setup();
	void setupWindow(Window* window);
	QMenuBar* menuBar(Window* window);

private slots:
	void onWindowFileDoubleClicked(const Path& path);
	void onWindowClosing();
};
