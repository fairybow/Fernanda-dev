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

QGroupBox* MenuBar::themesGroupBox()
{
	auto box = new QGroupBox(tr("Themes"));
	auto editor_themes = new ComboBox;
	auto window_themes = new ComboBox;
	addActionsToBoxes(editor_themes, m_actionGroups[GROUP_EDITOR_THEMES]);
	addActionsToBoxes(window_themes, m_actionGroups[GROUP_WINDOW_THEMES]);
	for (auto& combo_box : { editor_themes, window_themes }) {
		connect(combo_box, &ComboBox::currentIndexChanged, this, [&](int index) {
			combo_box->itemData(index).value<QAction*>()->trigger();
			});
	}
	auto labeled_editor_themes = Layout::container(editor_themes, nullptr, "Editor");
	auto labeled_window_themes = Layout::container(window_themes, nullptr, "Window");
	auto themes_layout = Layout::box(Layout::Line::Horizontally, { labeled_editor_themes, labeled_window_themes }, box);
	Layout::setUniformSpacing(themes_layout);
	return box;
}

QGroupBox* MenuBar::fontGroupBox()
{
	auto box = new QGroupBox(tr("Font"));
	auto mdi_area = new QMdiArea;
	addFontDialog(mdi_area);
	auto font_layout = Layout::box(Layout::Line::Horizontally, mdi_area, box);
	Layout::setUniformSpacing(font_layout);
	return box;
}

QGroupBox* MenuBar::editorGroupBox()
{
	/*auto wrap_modes_combo_box = new ComboBox;
	addActionsToBoxes(wrap_modes_combo_box, m_actionGroups[GROUP_WRAPS]);
	connect(wrap_modes_combo_box, &ComboBox::currentIndexChanged, this, [&](int index) {
		wrap_modes_combo_box->itemData(index).value<QAction*>()->trigger();
		});*/

		/*
		Wrap Modes (combo)
		LineHighlight, (rest check)
		LineNumberArea,
		Shadow
		*/

	auto box = new QGroupBox(tr("Editor"));
	auto tab_stops_slider = new Slider("Slider", Qt::Horizontal, nullptr, "Tab stop distance", true, "pixels", 10);
	tab_stops_slider->setRange(1, 30);
	tab_stops_slider->setValue(m_sliderValues[SLIDER_TABS]);
	auto editor_layout = Layout::box(Layout::Line::Horizontally, tab_stops_slider, box);

	connect(tab_stops_slider, &Slider::valueChanged, this, [&](int value) {
		setSelectedTabStop(value);
		emit askSetTabStop(value);
		});

	Layout::setUniformSpacing(editor_layout);
	return box;
}

QGroupBox* MenuBar::cursorGroupBox()
{
	// Cursor (all check box)
	/*CursorBlink,
	CursorBlock,
	CursorCenterOnScroll,
	CursorEnsureVisible,
	CursorTypewriter,*/

	//Layout::setUniformSpacing(...);
	return new QGroupBox;
}

QGroupBox* MenuBar::meterGroupBox()
{
	auto box = new QGroupBox(tr("Meter"));
	auto line_check_box = new QCheckBox("Line");
	auto column_check_box = new QCheckBox("Column");
	auto meter_separator = new QLabel(
		Utility::padString("/", 4));
	meter_separator->setAlignment(Qt::AlignCenter);
	auto lines_check_box = new QCheckBox("Lines");
	auto words_check_box = new QCheckBox("Words");
	auto characters_check_box = new QCheckBox("Characters");

	line_check_box->setChecked(m_checkBoxStates[CHECK_BOX_LINE_POS]);
	column_check_box->setChecked(m_checkBoxStates[CHECK_BOX_COL_POS]);
	lines_check_box->setChecked(m_checkBoxStates[CHECK_BOX_LINES]);
	words_check_box->setChecked(m_checkBoxStates[CHECK_BOX_WORDS]);
	characters_check_box->setChecked(m_checkBoxStates[CHECK_BOX_CHARS]);

	auto positions_layout = Layout::box(Layout::Line::Horizontally, { line_check_box, column_check_box });
	auto counts_layout = Layout::box(Layout::Line::Horizontally, { lines_check_box, words_check_box, characters_check_box });

	auto meter_layout = Layout::box(Layout::Line::Horizontally,
		nullptr, box);
	meter_layout->addLayout(positions_layout, 0);
	meter_layout->addWidget(meter_separator, 1);
	meter_layout->addLayout(counts_layout, 0);

	connect(line_check_box, &QCheckBox::stateChanged, this, [&](int state) {
		setCheckBoxLinePosition(state);
		emit askToggleLinePosition(state);
		});
	connect(column_check_box, &QCheckBox::stateChanged, this, [&](int state) {
		setCheckBoxColumnPosition(state);
		emit askToggleColumnPosition(state);
		});
	connect(lines_check_box, &QCheckBox::stateChanged, this, [&](int state) {
		setCheckBoxLineCount(state);
		emit askToggleLineCount(state);
		});
	connect(words_check_box, &QCheckBox::stateChanged, this, [&](int state) {
		setCheckBoxWordCount(state);
		emit askToggleWordCount(state);
		});
	connect(characters_check_box, &QCheckBox::stateChanged, this, [&](int state) {
		setCheckBoxCharacterCount(state);
		emit askToggleCharacterCount(state);
		});
	Layout::setUniformSpacing({ positions_layout, counts_layout, meter_layout });
	return box;
}

QGroupBox* MenuBar::toolsGroupBox()
{
	auto box = new QGroupBox(tr("Tools"));
	auto tool_layout = Layout::box(Layout::Line::Vertically, nullptr, box);

	auto pomodoro_timer_check_box = new QCheckBox(
		QString(Emoji::TOMATO) + " Pomodoro timer");
	auto stay_awake_check_box = new QCheckBox(
		QString(Emoji::TEACUP) + " Stay awake");
	auto always_on_top_check_box = new QCheckBox(
		QString(Emoji::PUSHPIN) + " Always on top");
	auto tool_check_boxes_layout = Layout::box(Layout::Line::Horizontally,
		{ pomodoro_timer_check_box, stay_awake_check_box, always_on_top_check_box });
	pomodoro_timer_check_box->setChecked(m_checkBoxStates[CHECK_BOX_POMODORO]);
	stay_awake_check_box->setChecked(m_checkBoxStates[CHECK_BOX_STAY_AWAKE]);
	always_on_top_check_box->setChecked(m_checkBoxStates[CHECK_BOX_ALWAYS_ON_TOP]);
	tool_layout->addLayout(tool_check_boxes_layout);

	auto pomodoro_times_slider = new Slider("Slider", Qt::Horizontal, nullptr, "Pomodoro interval", true, "minutes");
	pomodoro_times_slider->setRange(1, 60);
	pomodoro_times_slider->setValue(m_sliderValues[SLIDER_POMODORO] / 60);

	tool_layout->addWidget(pomodoro_times_slider);

	connect(pomodoro_timer_check_box, &QCheckBox::stateChanged, this, [&](int state) {
		setCheckBoxPomodoroTimer(state);
		emit askTogglePomodoroTimer(state);
		});
	connect(stay_awake_check_box, &QCheckBox::stateChanged, this, [&](int state) {
		setCheckBoxStayAwake(state);
		emit askToggleStayAwake(state);
		});
	connect(always_on_top_check_box, &QCheckBox::stateChanged, this, [&](int state) {
		setCheckBoxAlwaysOnTop(state);
		emit askToggleAlwaysOnTop(state);
		});
	connect(pomodoro_times_slider, &Slider::valueChanged, this, [&](int value) {
		setSelectedPomodoroTime(value * 60);
		emit askSetPomodoroTime(value * 60);
		});
	Layout::setUniformSpacing({ tool_layout, tool_check_boxes_layout });
	return box;
}


void MenuBar::appearanceDialog()
{
	QDialog dialog(this);
	auto full_layout = Layout::grid(nullptr, &dialog);
	full_layout->addWidget(themesGroupBox(), 0, 0, 1, 2);
	full_layout->addWidget(fontGroupBox(), 1, 0, 5, 2);
	full_layout->addWidget(editorGroupBox(), 0, 3, 1, 2);
	full_layout->addWidget(meterGroupBox(), 4, 3, 1, 2);
	full_layout->addWidget(toolsGroupBox(), 5, 3, 1, 2);
	Layout::setMinAndMaxSize(&dialog, 800, 450);
	dialog.exec();
}
