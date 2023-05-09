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

	// Misc.
	auto tab_stops_slider = new QSlider(Qt::Horizontal);
	tab_stops_slider->setObjectName(objectName() + "slider");
	tab_stops_slider->setRange(20, 80);
	auto tab_stops_container = Layout::labeledContainer("Settings:", tab_stops_slider);

	// Tools
	auto pomodoro_times_slider = new Slider(Qt::Horizontal);
	pomodoro_times_slider->setObjectName(objectName() + "slider");
	pomodoro_times_slider->setRange(300, 1800);
	pomodoro_times_slider->setSingleStep(100);
	auto pomodoro_times_container = Layout::labeledContainer("Tools:", pomodoro_times_slider);

	// Font box
	auto font_box_area = new QMdiArea;
	addFontDialog(font_box_area);

	// Right side
	auto settings_layout = Layout::box({ tab_stops_container, pomodoro_times_container }, nullptr);

	// Left side
	auto left_hand_layout = Layout::box(nullptr, nullptr);
	auto combo_boxes_layout = Layout::box({ editor_themes_container, window_themes_container }, nullptr, Layout::Line::Horizontally);
	left_hand_layout->addLayout(combo_boxes_layout);
	left_hand_layout->addWidget(font_box_area);

	// Full layout // grid?
	auto full_layout = Layout::box(nullptr, &dialog, Layout::Line::Horizontally, { 10, 10, 10, 10 });
	full_layout->addLayout(left_hand_layout);
	full_layout->addLayout(settings_layout);

	Layout::setMinAndMaxSize(&dialog, 1000, 400);

	dialog.exec();
}
