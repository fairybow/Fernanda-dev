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
	m_actionGroups[GROUP_EDITOR_THEMES] = ActionGroup::fromQrc(QRC_EDITOR,
		".fernanda_editor", user_data_path, this, [&] {

			auto selection = selectedEditorTheme();
			if (selection == nullptr) return;
			emit askStyleEditor(Path::toStdFs(selection->data()));

		});

	m_actionGroups[GROUP_WINDOW_THEMES] = ActionGroup::fromQrc(QRC_MAIN_WINDOW,
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
	m_actionGroups[GROUP_WRAPS] = ActionGroup::fromBespoke(wrap_modes, this, [&] {

		auto selection = selectedWrapMode();
		if (selection == nullptr) return;
		emit askSetWrapMode(selection->data().toString());

		});

	ActionGroup::BespokeList indicator_positions;
	indicator_positions << ActionGroup::bespoke("Top");
	indicator_positions << ActionGroup::bespoke("Bottom");
	m_actionGroups[GROUP_INDICATOR_POS] = ActionGroup::fromBespoke(indicator_positions, this, [&] {

		auto selection = selectedIndicatorPosition();
		if (selection == nullptr) return;
		emit askSetIndicatorPosition(selection->data().toString());

		});

	ActionGroup::BespokeList preview_types;
	preview_types << ActionGroup::bespoke("Fountain");
	preview_types << ActionGroup::bespoke("Markdown");
	m_actionGroups[GROUP_PREVIEW] = ActionGroup::fromBespoke(preview_types, this, [&] {

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
	// split-up

	QDialog dialog(this);

	// Themes
	auto themes_box = new QGroupBox(tr("Themes"));
	auto editor_themes_combo_box = new ComboBox;
	auto window_themes_combo_box = new ComboBox;
	addActionsToBoxes(editor_themes_combo_box, m_actionGroups[GROUP_EDITOR_THEMES]);
	addActionsToBoxes(window_themes_combo_box, m_actionGroups[GROUP_WINDOW_THEMES]);
	for (auto& combo_box : { editor_themes_combo_box, window_themes_combo_box }) {
		connect(combo_box, &ComboBox::currentIndexChanged, this, [&](int index) {
			combo_box->itemData(index).value<QAction*>()->trigger();
			});
	}
	auto themes_layout = Layout::box(Layout::Line::Horizontally, { editor_themes_combo_box, window_themes_combo_box }, themes_box);

	// Font box
	auto font_box = new QGroupBox(tr("Font"));
	auto font_box_area = new QMdiArea;
	addFontDialog(font_box_area);
	auto font_layout = Layout::box(Layout::Line::Horizontally, font_box_area, font_box);

	// Editor settings
	auto editor_box = new QGroupBox(tr("Editor"));
	auto tab_stops_slider = new Slider("Slider", Qt::Horizontal, nullptr, "Tab stop distance", true, "pixels", 10);
	tab_stops_slider->setRange(1, 30);
	tab_stops_slider->setValue(m_sliderValues[SLIDER_TABS]);
	auto editor_layout = Layout::box(Layout::Line::Horizontally, tab_stops_slider, editor_box);

	// Tool settings
	auto tool_box = new QGroupBox(tr("Tools"));
	auto tool_layout = Layout::box(Layout::Line::Vertically, nullptr, tool_box);

	auto pomodoro_timer_check_box = new QCheckBox(QString(TOMATO) + " Pomodoro timer");
	auto stay_awake_check_box = new QCheckBox(QString(TEACUP) + " Stay awake");
	auto always_on_top_check_box = new QCheckBox(QString(PUSHPIN) + " Always on top");
	auto tool_check_boxes_layout = Layout::box(Layout::Line::Horizontally,
		{ pomodoro_timer_check_box, stay_awake_check_box, always_on_top_check_box });

	tool_layout->addLayout(tool_check_boxes_layout);

	auto pomodoro_times_slider = new Slider("Slider", Qt::Horizontal, nullptr, "Pomodoro interval", true, "minutes");
	pomodoro_times_slider->setRange(1, 60);
	pomodoro_times_slider->setValue(m_sliderValues[SLIDER_POMODORO] / 60);

	tool_layout->addWidget(pomodoro_times_slider);

	connect(tab_stops_slider, &Slider::valueChanged, this, [&](int value) {
		setSelectedTabStop(value);
		emit askSetTabStop(value);
		});
	connect(pomodoro_timer_check_box, &QCheckBox::stateChanged, this, [&](int state) {
		//set(value);
		//emit sig(value);
		qDebug() << state << pomodoro_timer_check_box;
		});
	connect(stay_awake_check_box, &QCheckBox::stateChanged, this, [&](int state) {
		//set(value);
		//emit sig(value);
		qDebug() << state << stay_awake_check_box;
		});
	connect(always_on_top_check_box, &QCheckBox::stateChanged, this, [&](int state) {
		//set(value);
		//emit sig(value);
		qDebug() << state << always_on_top_check_box;
		});
	connect(pomodoro_times_slider, &Slider::valueChanged, this, [&](int value) {
		setSelectedPomodoroTime(value * 60);
		emit askSetPomodoroTime(value * 60);
		});

	//

	auto full_layout = Layout::grid(nullptr, &dialog);

	full_layout->addWidget(themes_box, 0, 0, 1, 2);
	full_layout->addWidget(font_box, 1, 0, 3, 2);
	full_layout->addWidget(editor_box, 0, 3, 1, 2);
	full_layout->addWidget(tool_box, 3, 3, 1, 2);

	Layout::setMinAndMaxSize(&dialog, 800, 400);
	Layout::setUniformSpacing({ themes_layout, font_layout, editor_layout, tool_layout, tool_check_boxes_layout, full_layout });

	font_box_area->setFixedWidth(dialog.width() / 2);

	dialog.exec();
}
