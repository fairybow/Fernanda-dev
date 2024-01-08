#pragma once

#include "common/Path.hpp"
#include "window/Window.h"
#include "window/WindowSettings.h"

#include <QDebug>
#include <QDir>
#include <QList>

class Fernanda : public QObject
{
	Q_OBJECT

public:
	Fernanda(bool isDev);
	~Fernanda() { qDebug() << __FUNCTION__; }

	Window* newWindow();

private:
	static constexpr char SETTINGS_INI[] = ".fernanda/Settings.ini";
	const Path& m_iniPath = Path(QDir::homePath()) / SETTINGS_INI;

	bool m_isDev;
	WindowSettings* m_windowSettings = new WindowSettings(m_iniPath, this);
	QList<Window*> m_windows;

	void setup();
	void setupWindow(Window* window);

private slots:
	void onWindowFileDoubleClicked(const Path& path);
	void onWindowClosing();
	void onWindowSettingChecked(bool checked, WindowSettings::Type type);
};
