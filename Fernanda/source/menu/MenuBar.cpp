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
		connect(combo_box, &ComboBox::currentIndexChanged, this, [&, combo_box](int index) {
			combo_box->itemData(index).value<QAction*>()->trigger();
			});
	}

	auto labeled_editor_themes = Layout::container(editor_themes, nullptr, "Editor");
	auto labeled_window_themes = Layout::container(window_themes, nullptr, "Window");
	auto layout = Layout::box(Layout::Line::Horizontally, { labeled_editor_themes, labeled_window_themes }, box);
	Layout::setUniformSpacing(layout);
	return box;
}

QGroupBox* MenuBar::fontGroupBox()
{
	auto box = new QGroupBox(tr("Font"));
	auto mdi_area = new QMdiArea;
	addFontDialog(mdi_area);
	auto layout = Layout::box(Layout::Line::Horizontally, mdi_area, box);
	Layout::setUniformSpacing(layout);
	return box;
}

QGroupBox* MenuBar::editorGroupBox()
{
	auto box = new QGroupBox(tr("Editor"));

	auto wrap_modes = new ComboBox;
	addActionsToBoxes(wrap_modes, m_actionGroups[GROUP_WRAPS]);

	auto tab_stops_slider = new Slider("Slider", Qt::Horizontal, nullptr, "Tab stop distance", true, "pixels", 10);
	tab_stops_slider->setRange(1, 30);
	tab_stops_slider->setValue(m_sliderValues[SLIDER_TABS]);

	auto line_highlight = new QCheckBox("Line highlight");
	auto line_number_area = new QCheckBox("Line number area");
	auto shadow = new QCheckBox("Shadow");

	line_highlight->setChecked(m_checkBoxStates[CHECK_BOX_LINE_HIGHLIGHT]);
	line_number_area->setChecked(m_checkBoxStates[CHECK_BOX_LINE_NUMBERS]);
	shadow->setChecked(m_checkBoxStates[CHECK_BOX_SHADOW]);

	connect(wrap_modes, &ComboBox::currentIndexChanged, this, [&, wrap_modes](int index) {
		wrap_modes->itemData(index).value<QAction*>()->trigger();
		});
	connect(tab_stops_slider, &Slider::valueChanged, this, [&](int value) {
		setSelectedTabStop(value);
		emit askSetTabStop(value);
		});
	connect(line_highlight, &QCheckBox::stateChanged, this, [&](int state) {
		setCheckBoxLineHighlight(state);
		emit askToggleLineHighlight(state);
		});
	connect(line_number_area, &QCheckBox::stateChanged, this, [&](int state) {
		setCheckBoxLineNumbers(state);
		emit askToggleLineNumbers(state);
		});
	connect(shadow, &QCheckBox::stateChanged, this, [&](int state) {
		setCheckBoxShadow(state);
		emit askToggleShadow(state);
		});

	auto layout = Layout::box(Layout::Line::Vertically, { wrap_modes }, box);
	auto middle_layout = Layout::box(Layout::Line::Horizontally, { line_highlight, line_number_area, shadow });
	layout->addLayout(middle_layout);
	layout->addWidget(tab_stops_slider);
	layout->addWidget(cursorGroupBox());
	Layout::setUniformSpacing({ middle_layout, layout });
	return box;
}

QGroupBox* MenuBar::cursorGroupBox()
{
	auto box = new QGroupBox(tr("Cursor"));

	auto blink = new QCheckBox("Blink");
	auto block = new QCheckBox("Block");
	auto center_on_scroll = new QCheckBox("Center on scroll");
	auto ensure_visible = new QCheckBox("Ensure visible");
	auto typewriter = new QCheckBox("Typewriter");

	blink->setChecked(m_checkBoxStates[CHECK_BOX_BLINK]);
	block->setChecked(m_checkBoxStates[CHECK_BOX_BLOCK]);
	center_on_scroll->setChecked(m_checkBoxStates[CHECK_BOX_CENTER_ON_SCROLL]);
	ensure_visible->setChecked(m_checkBoxStates[CHECK_BOX_ENSURE_VISIBLE]);
	typewriter->setChecked(m_checkBoxStates[CHECK_BOX_TYPEWRITER]);

	connect(blink, &QCheckBox::stateChanged, this, [&](int state) {
		setCheckBoxBlink(state);
		emit askToggleBlink(state);
		});
	connect(block, &QCheckBox::stateChanged, this, [&](int state) {
		setCheckBoxBlock(state);
		emit askToggleBlock(state);
		});
	connect(center_on_scroll, &QCheckBox::stateChanged, this, [&](int state) {
		setCheckBoxCenterOnScroll(state);
		emit askToggleCenterOnScroll(state);
		});
	connect(ensure_visible, &QCheckBox::stateChanged, this, [&](int state) {
		setCheckBoxEnsureVisible(state);
		emit askToggleEnsureVisible(state);
		});
	connect(typewriter, &QCheckBox::stateChanged, this, [&](int state) {
		setCheckBoxTypewriter(state);
		emit askToggleTypewriter(state);
		});

	auto layout = Layout::box(Layout::Line::Horizontally, { blink, block, center_on_scroll, ensure_visible, typewriter }, box);
	Layout::setUniformSpacing(layout);
	return box;
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
	auto layout = Layout::box(Layout::Line::Horizontally,
		nullptr, box);
	layout->addLayout(positions_layout, 0);
	layout->addWidget(meter_separator, 1);
	layout->addLayout(counts_layout, 0);

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

	Layout::setUniformSpacing({ positions_layout, counts_layout, layout });
	return box;
}

QGroupBox* MenuBar::toolsGroupBox()
{
	auto box = new QGroupBox(tr("Tools"));

	auto pomodoro_timer_check_box = new QCheckBox(
		QString(Emoji::TOMATO) + " Pomodoro timer");
	auto stay_awake_check_box = new QCheckBox(
		QString(Emoji::TEACUP) + " Stay awake");
	auto always_on_top_check_box = new QCheckBox(
		QString(Emoji::PUSHPIN) + " Always on top");
	auto check_boxes_layout = Layout::box(Layout::Line::Horizontally,
		{ pomodoro_timer_check_box, stay_awake_check_box, always_on_top_check_box });

	pomodoro_timer_check_box->setChecked(m_checkBoxStates[CHECK_BOX_POMODORO]);
	stay_awake_check_box->setChecked(m_checkBoxStates[CHECK_BOX_STAY_AWAKE]);
	always_on_top_check_box->setChecked(m_checkBoxStates[CHECK_BOX_ALWAYS_ON_TOP]);

	auto pomodoro_times_slider = new Slider("Slider", Qt::Horizontal, nullptr, "Pomodoro interval", true, "minutes");
	pomodoro_times_slider->setRange(1, 60);
	pomodoro_times_slider->setValue(m_sliderValues[SLIDER_POMODORO] / 60);

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

	auto layout = Layout::box(Layout::Line::Vertically, nullptr, box);
	layout->addLayout(check_boxes_layout);
	layout->addWidget(pomodoro_times_slider);
	Layout::setUniformSpacing({ layout, check_boxes_layout });
	return box;
}

QGroupBox* MenuBar::mixedGroupBox()
{
	auto box = new QGroupBox(tr(""));
	// indicator position
	// preview type
	//Layout::setUniformSpacing(...);
	return box;
}

void MenuBar::appearanceDialog()
{
	QDialog dialog(this);

	auto full_layout = Layout::grid(nullptr, &dialog);
	full_layout->addWidget(themesGroupBox(), 0, 0, 1, 2);
	full_layout->addWidget(fontGroupBox(), 1, 0, 5, 2);
	full_layout->addWidget(editorGroupBox(), 0, 3, 3, 2);
	full_layout->addWidget(meterGroupBox(), 3, 3, 1, 2);
	full_layout->addWidget(toolsGroupBox(), 4, 3, 1, 2);
	full_layout->addWidget(mixedGroupBox(), 5, 3, 1, 2);

	dialog.setFixedSize(800, 450);
	Layout::setUniformSpacing(full_layout);
	dialog.exec();
}
