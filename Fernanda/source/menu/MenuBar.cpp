#include "MenuBar.h"

MenuBar::MenuBar(const char* name, QWidget* parent)
	: QMenuBar(parent)
{
	setObjectName(name);

	// functions to collect and store action groups for available resources (themes, fonts, etc.)
}

void MenuBar::makeSubmenus()
{
	view();
	help();
}

void MenuBar::view()
{
	auto appearance = new QAction(tr("&Appearance..."), this);
	connect(appearance, &QAction::triggered, this, &MenuBar::appearanceDialog);
	auto menu = addMenu(tr("&View"));
	for (const auto& action : { appearance })
		menu->addAction(action);
}

void MenuBar::help()
{
	auto about = new QAction(tr("&About..."), this);
	auto check_for_updates = new QAction(tr("&Check for updates..."), this);
	connect(about, &QAction::triggered, this, [&] {
		if (!MenuPopup::about(this)) return;
		MenuPopup::version(this);
		});
	connect(check_for_updates, &QAction::triggered, this, [&] {
		MenuPopup::version(this);
		});
	auto menu = addMenu(tr("&Help"));
	for (const auto& action : { about, check_for_updates })
		menu->addAction(action);
}

void MenuBar::appearanceDialog()
{
	//
}
