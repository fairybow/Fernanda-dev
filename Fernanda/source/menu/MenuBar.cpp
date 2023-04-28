#include "MenuBar.h"

MenuBar::MenuBar(QWidget* parent)
	: QMenuBar(parent)
{
	//setObjectName(name);
}

void MenuBar::makeSubmenus()
{
	help();
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
	auto help = addMenu(tr("&Help"));
	for (const auto& action : { about, check_for_updates })
		help->addAction(action);
}
