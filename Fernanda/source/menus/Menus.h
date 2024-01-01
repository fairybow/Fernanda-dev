#pragma once

#include <QDebug>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>

class Menus : public QObject
{
	Q_OBJECT

public:
	Menus(QObject* parent = nullptr);
	~Menus() { qDebug() << __FUNCTION__; }

	QMenu* file(QMenuBar* menuBar, QMainWindow* mainWindow);
	QMenu* view(QMenuBar* menuBar, QMainWindow* mainWindow);
	QMenu* help(QMenuBar* menuBar, QMainWindow* mainWindow);
	QMenu* dev(QMenuBar* menuBar, QMainWindow* mainWindow);
};
