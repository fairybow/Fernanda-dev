#include "Menus.h"

#include <QAction>

Menus::Menus(QObject* parent)
	: QObject(parent)
{}

QMenu* Menus::file(QMenuBar* menuBar, QMainWindow* mainWindow)
{
	auto menu = new QMenu("File", menuBar);
	auto test = new QAction("Print window memory address", menuBar);
	connect(test, &QAction::triggered, mainWindow, [=] { qDebug() << mainWindow; });
	menu->addAction(test);

	return menu;
}

QMenu* Menus::view(QMenuBar* menuBar, QMainWindow* mainWindow)
{
	auto menu = new QMenu("View", menuBar);
	auto test = new QAction("Print window memory address", menuBar);
	connect(test, &QAction::triggered, mainWindow, [=] { qDebug() << mainWindow; });
	menu->addAction(test);

	return menu;
}

QMenu* Menus::help(QMenuBar* menuBar, QMainWindow* mainWindow)
{
	auto menu = new QMenu("Help", menuBar);
	auto test = new QAction("Print window memory address", menuBar);
	connect(test, &QAction::triggered, mainWindow, [=] { qDebug() << mainWindow; });
	menu->addAction(test);

	return menu;
}

QMenu* Menus::dev(QMenuBar* menuBar, QMainWindow* mainWindow)
{
	auto menu = new QMenu("Dev", menuBar);
	auto test = new QAction("Print window memory address", menuBar);
	connect(test, &QAction::triggered, mainWindow, [=] { qDebug() << mainWindow; });
	menu->addAction(test);

	return menu;
}
