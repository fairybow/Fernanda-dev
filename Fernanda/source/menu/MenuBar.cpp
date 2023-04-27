#include "MenuBar.h"

MenuBar::MenuBar(const char* name, QWidget* parent)
	: QMenuBar(parent)
{
	setObjectName(name);
}

void MenuBar::makeSubmenus()
{
	help();
}

void MenuBar::help()
{
	auto about = new QAction(tr("&About..."), this);
	auto check_for_updates = new QAction(tr("&Check for updates..."), this);
	auto shortcuts = new QAction(tr("&Shortcuts..."), this);
	auto documents = new QAction(tr("&Documents..."), this);
	auto installation_folder = new QAction(tr("&Installation folder..."), this);
	auto user_data_folder = new QAction(tr("&User data..."), this);
	auto create_sample_project = new QAction(tr("&Create sample project"), this);
	auto create_sample_themes = new QAction(tr("&Create sample themes..."), this);
	connect(about, &QAction::triggered, this, [&] {
		//if (!MenuPopup::about(this)) return;
		//MenuPopup::versionCheck(this);
		});
	connect(check_for_updates, &QAction::triggered, this, [&] {
		//MenuPopup::versionCheck(this);
		});
	connect(shortcuts, &QAction::triggered, this, [&] {
		//MenuPopup::shortcuts(this);
		});
	connect(documents, &QAction::triggered, this, [&] {
		//openLocalFolder(UserData::doThis(UserData::Operation::GetDocuments));
		});
	connect(installation_folder, &QAction::triggered, this, [&] {
		//openLocalFolder(Path::toStdFs(QCoreApplication::applicationDirPath()).parent_path());
		});
	connect(user_data_folder, &QAction::triggered, this, [&] {
		//openLocalFolder(UserData::doThis(UserData::Operation::GetUserData));
		});
	connect(create_sample_project, &QAction::triggered, this, [&] {
		//askStorySample();
		});
	connect(create_sample_themes, &QAction::triggered, this, [&] {
		//sampleThemes();
		});
	auto help = addMenu(tr("&Help"));
	for (const auto& action : { about, check_for_updates, shortcuts, help->addSeparator() })
		help->addAction(action);
	auto open = help->addMenu(tr("&Open"));
	for (const auto& action : { documents, installation_folder, user_data_folder })
		open->addAction(action);
	for (const auto& action : { help->addSeparator(), create_sample_project, create_sample_themes })
		help->addAction(action);
}
