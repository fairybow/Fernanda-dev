#include "MenuBar.h"

MenuBar::MenuBar(const char* name, bool isDev, QWidget* parent)
	: Widget(name, parent), m_isDev(isDev)
{
	makeActionGroups();
}

void MenuBar::makeSubmenus()
{
	view();
	help();
}

void MenuBar::makeActionGroups()
{
	auto user_data_path = emit getUserDataPath();
	m_actionGroups[EDITOR_THEMES] = ActionGroup::fromQrc(QRC_EDITOR,
		".fernanda_editor", user_data_path, this, [&] {

			auto selection = selectedEditorTheme();
			if (selection == nullptr) return;
			emit askStyleEditor(Path::toStdFs(selection->data()));

		});
	m_actionGroups[WINDOW_THEMES] = ActionGroup::fromQrc(QRC_MAIN_WINDOW,
		".fernanda_window", user_data_path, this, [&] {

			auto selection = selectedWindowTheme();
			if (selection == nullptr) return;
			emit askStyleWindow(Path::toStdFs(selection->data()));

		});

	// check that `user_data_path` can be empty

	// bespokes

}

void MenuBar::setSelectedGroupAction(ActionGroup* actionGroup, const StdFsPath& path)
{
	for (auto i = 0; i < actionGroup->actions().count(); ++i) {
		auto action = actionGroup->actions().at(i);
		if (Path::toStdFs(action->data()) == path) {
			action->setChecked(true);
			return;
		}
	}
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
		if (!Popup::about(this)) return;
		Popup::version(this);
		});
	connect(check_for_updates, &QAction::triggered, this, [&] {
		Popup::version(this);
		});
	auto menu = addMenu(tr("&Help"));
	for (const auto& action : { about, check_for_updates })
		menu->addAction(action);
}

void MenuBar::addActionsToBoxes(QComboBox* comboBox, ActionGroup* actionGroup)
{
	for (auto i = 0; i < actionGroup->actions().count(); ++i) {
		auto action = actionGroup->actions().at(i);
		comboBox->addItem(action->text(), QVariant::fromValue(action));
		if (action == actionGroup->checkedAction())
			comboBox->setCurrentIndex(i);
	}
}

void MenuBar::addFontDialog(QMdiArea* multiDocArea)
{
	auto dialog = multiDocArea->addSubWindow(fontDialog());
	dialog->setWindowFlags(Qt::FramelessWindowHint);
	dialog->showMaximized();
}

LiveFontDialog* MenuBar::fontDialog()
{
	auto dialog = new LiveFontDialog(emit getUserFont(), this);
	dialog->setOptions(LiveFontDialog::NoButtons);
	connect(dialog, &LiveFontDialog::currentFontChanged, this, [&](const QFont& font) {
		emit askChangeFont(font);
		});
	return dialog;
}

void MenuBar::appearanceDialog()
{
	QDialog dialog(this);
	auto editor_themes_box = new QComboBox;
	auto window_themes_box = new QComboBox;
	addActionsToBoxes(editor_themes_box, m_actionGroups[EDITOR_THEMES]);
	addActionsToBoxes(window_themes_box, m_actionGroups[WINDOW_THEMES]);
	auto editor_themes_container = Layout::labeledContainer("Editor theme:", editor_themes_box);
	auto window_themes_container = Layout::labeledContainer("Window theme:", window_themes_box);
	for (auto& themes_box : { editor_themes_box, window_themes_box }) {
		connect(themes_box, &QComboBox::currentIndexChanged, this, [&](int index) {
			themes_box->itemData(index).value<QAction*>()->trigger();
			});
	}
	auto font_box_area = new QMdiArea;
	addFontDialog(font_box_area);
	auto layout = Layout::box(nullptr, &dialog, Layout::Line::Vertically, {10, 10, 10, 10});
	auto combo_boxes_layout = Layout::box({ editor_themes_container, window_themes_container }, nullptr, Layout::Line::Horizontally);
	layout->addLayout(combo_boxes_layout);
	layout->addWidget(font_box_area);
	Layout::setMinAndMaxSize(&dialog, 600, 400);
	dialog.exec();
}
