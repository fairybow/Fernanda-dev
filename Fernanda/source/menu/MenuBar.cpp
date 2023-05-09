#include "MenuBar.h"

MenuBar::MenuBar(const char* name, bool isDev, QWidget* parent)
	: Widget(name, parent), m_isDev(isDev)
{
	makeActionGroups();
	makeBespokeActionGroups();
}

void MenuBar::makeSubmenus()
{
	view();
	help();
}

void MenuBar::makeActionGroups() // check that `user_data_path` can be empty
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
}

void MenuBar::makeBespokeActionGroups()
{
	ActionGroup::BespokeList wrap_modes;
	wrap_modes << ActionGroup::bespoke("NoWrap", "No wrap");
	wrap_modes << ActionGroup::bespoke("WordWrap", "Wrap at word boundaries");
	wrap_modes << ActionGroup::bespoke("WrapAnywhere", "Wrap anywhere");
	wrap_modes << ActionGroup::bespoke("WrapAt", "Wrap at word boundaries or anywhere");
	m_actionGroups[WRAPS] = ActionGroup::fromBespoke(wrap_modes, this, [&] {

		auto selection = selectedWrapMode();
		if (selection == nullptr) return;
		emit askSetWrapMode(selection->data().toString());

		});

	ActionGroup::BespokeList indicator_positions;
	indicator_positions << ActionGroup::bespoke("Top");
	indicator_positions << ActionGroup::bespoke("Bottom");
	m_actionGroups[INDICATOR_POS] = ActionGroup::fromBespoke(indicator_positions, this, [&] {

		auto selection = selectedIndicatorPosition();
		if (selection == nullptr) return;
		emit askSetIndicatorPosition(selection->data().toString());

		});

	ActionGroup::BespokeList preview_types;
	preview_types << ActionGroup::bespoke("Fountain");
	preview_types << ActionGroup::bespoke("Markdown");
	m_actionGroups[PREVIEW] = ActionGroup::fromBespoke(preview_types, this, [&] {

		auto selection = selectedPreviewType();
		if (selection == nullptr) return;
		emit askSetPreviewType(selection->data().toString());

		});
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

	// Themes
	auto themes_box = new QGroupBox(tr("Themes:"));
	auto editor_themes_combo_box = new ComboBox;
	auto window_themes_combo_box = new ComboBox;
	addActionsToBoxes(editor_themes_combo_box, m_actionGroups[EDITOR_THEMES]);
	addActionsToBoxes(window_themes_combo_box, m_actionGroups[WINDOW_THEMES]);
	for (auto& combo_box : { editor_themes_combo_box, window_themes_combo_box }) {
		connect(combo_box, &ComboBox::currentIndexChanged, this, [&](int index) {
			combo_box->itemData(index).value<QAction*>()->trigger();
			});
	}
	auto themes_layout = Layout::box({ editor_themes_combo_box, window_themes_combo_box }, themes_box, Layout::Line::Horizontally);

	// Font box
	auto font_box = new QGroupBox(tr("Font:"));
	auto font_box_area = new QMdiArea;
	addFontDialog(font_box_area);
	auto font_layout = Layout::box(font_box_area, font_box);

	// Editor settings
	auto editor_box = new QGroupBox(tr("Editor:"));
	auto tab_stops_slider = new Slider(Qt::Horizontal, nullptr, "Tab stop distance:");
	//tab_stops_slider->setObjectName(objectName() + "slider");
	//tab_stops_slider->setRange(20, 80);
	auto editor_layout = Layout::box(tab_stops_slider, editor_box);

	// Tool settings
	auto tool_box = new QGroupBox(tr("Tools:"));
	auto pomodoro_times_slider = new Slider(Qt::Horizontal, nullptr, "Pomodoro interval:");
	//pomodoro_times_slider->setObjectName(objectName() + "slider");
	//pomodoro_times_slider->setRange(300, 1800);
	//pomodoro_times_slider->setSingleStep(100);
	auto tool_layout = Layout::box(pomodoro_times_slider, tool_box);




	//auto full_layout = Layout::box({ themes_box, font_box }, &dialog);
	//auto full_layout = Layout::box({ themes_box, font_box }, &dialog);
	//auto full_layout = Layout::box({ themes_box, font_box }, &dialog);


	auto full_layout = Layout::grid(nullptr, &dialog);

	full_layout->addWidget(themes_box, 0, 0, 1, 2);
	full_layout->addWidget(font_box, 1, 0, 3, 2);
	full_layout->addWidget(editor_box, 0, 3, 1, 2);
	full_layout->addWidget(tool_box, 3, 3, 1, 2);

	Layout::setMinAndMaxSize(&dialog, 800, 400);
	Layout::setUniformSpacing({ themes_layout, font_layout, editor_layout, tool_layout, full_layout });

	font_box_area->setFixedWidth(dialog.width() / 2);

	dialog.exec();
}
