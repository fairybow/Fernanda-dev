#pragma once

#include "../common/Emoji.hpp"
#include "../common/Layout.hpp"
#include "../common/StringTools.hpp"
#include "../common/Widget.hpp"
#include "ActionGroup.h"
#include "ComboBox.hpp"
#include "LiveFontDialog.hpp"
#include "Popup.hpp"
#include "Slider.hpp"

#include <QCheckBox>
#include <QCoreApplication>
#include <QDesktopServices>
#include <QDialog>
#include <QFileDialog>
#include <QGroupBox>
#include <QMenuBar>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QVector>
#include <QWidgetAction>

#include <filesystem>
#include <map>

class MenuBar : public Widget<QMenuBar>
{
	using StdFsPath = std::filesystem::path;

	Q_OBJECT

public:
	MenuBar(const char* name, StdFsPath userData, StdFsPath userDocuments, bool isDev = false, QWidget* parent = nullptr);

	void makeSubmenus();

	StdFsPath defaultEditorTheme() const { return Path::toStdFs(QRC_EDITOR) / "Snooze.fernanda_editor"; }
	StdFsPath defaultWindowTheme() const { return Path::toStdFs(QRC_MAIN_WINDOW) / "Light.fernanda_window"; }

	void setCheckBoxEditorTheme(bool state) { m_checkBoxStates[CHECK_BOX_EDITOR_THEME] = state; }
	void setCheckBoxWindowTheme(bool state) { m_checkBoxStates[CHECK_BOX_WINDOW_THEME] = state; }
	void setSelectedEditorTheme(const StdFsPath& path) { setGroupSelectedAction(m_actionGroups[GROUP_EDITOR_THEMES], path); }
	void setSelectedWindowTheme(const StdFsPath& path) { setGroupSelectedAction(m_actionGroups[GROUP_WINDOW_THEMES], path); }
	void setSelectedTabStop(int pixels) { m_sliderValues[SLIDER_TABS] = pixels; }
	void setSelectedWrapMode(const QString& mode) { setBespokeGroupSelectedAction(m_actionGroups[GROUP_WRAPS], mode); }
	void setCheckBoxLineHighlight(bool state) { m_checkBoxStates[CHECK_BOX_LINE_HIGHLIGHT] = state; }
	void setCheckBoxLineNumbers(bool state) { m_checkBoxStates[CHECK_BOX_LINE_NUMBERS] = state; }
	void setCheckBoxShadow(bool state) { m_checkBoxStates[CHECK_BOX_SHADOW] = state; }
	void setCheckBoxBlink(bool state) { m_checkBoxStates[CHECK_BOX_BLINK] = state; }
	void setCheckBoxBlock(bool state) { m_checkBoxStates[CHECK_BOX_BLOCK] = state; }
	void setCheckBoxCenterOnScroll(bool state) { m_checkBoxStates[CHECK_BOX_CENTER_ON_SCROLL] = state; }
	void setCheckBoxEnsureVisible(bool state) { m_checkBoxStates[CHECK_BOX_ENSURE_VISIBLE] = state; }
	void setCheckBoxTypewriter(bool state) { m_checkBoxStates[CHECK_BOX_TYPEWRITER] = state; }
	void setCheckBoxLinePosition(bool state) { m_checkBoxStates[CHECK_BOX_LINE_POS] = state; }
	void setCheckBoxColumnPosition(bool state) { m_checkBoxStates[CHECK_BOX_COL_POS] = state; }
	void setCheckBoxLineCount(bool state) { m_checkBoxStates[CHECK_BOX_LINES] = state; }
	void setCheckBoxWordCount(bool state) { m_checkBoxStates[CHECK_BOX_WORDS] = state; }
	void setCheckBoxCharacterCount(bool state) { m_checkBoxStates[CHECK_BOX_CHARS] = state; }
	void setCheckBoxPomodoroTimer(bool state) { m_checkBoxStates[CHECK_BOX_POMODORO] = state; }
	void setCheckBoxStayAwake(bool state) { m_checkBoxStates[CHECK_BOX_STAY_AWAKE] = state; }
	void setCheckBoxAlwaysOnTop(bool state) { m_checkBoxStates[CHECK_BOX_ALWAYS_ON_TOP] = state; }
	void setSelectedPomodoroTime(int timeInSeconds) { m_sliderValues[SLIDER_POMODORO] = timeInSeconds; }
	void setCheckBoxIndicator(bool state) { m_checkBoxStates[CHECK_BOX_INDICATOR] = state; }
	void setSelectedIndicatorAlignment(const QString& alignment) { setBespokeGroupSelectedAction(m_actionGroups[GROUP_INDICATOR_ALIGN], alignment); }
	//void setSelectedPreviewType(const QString& type) { setBespokeGroupSelectedAction(m_actionGroups[GROUP_PREVIEW], type); }

signals:
	QFont getUserFont();
	void askOpenNewFile(StdFsPath path);
	void askOpenFile(StdFsPath path);
	void askSaveFile();
	void askToggleEditorTheme(bool state);
	void askToggleWindowTheme(bool state);
	void askStyleEditor(StdFsPath path);
	void askStyleWindow(StdFsPath path);
	void askChangeFont(const QFont& font);
	void askSetTabStop(int pixels);
	void askSetWrapMode(const QString& mode);
	void askToggleLineHighlight(bool state);
	void askToggleLineNumbers(bool state);
	void askToggleShadow(bool state);
	void askToggleBlink(bool state);
	void askToggleBlock(bool state);
	void askToggleCenterOnScroll(bool state);
	void askToggleEnsureVisible(bool state);
	void askToggleTypewriter(bool state);
	void askToggleLinePosition(bool state);
	void askToggleColumnPosition(bool state);
	void askToggleLineCount(bool state);
	void askToggleWordCount(bool state);
	void askToggleCharacterCount(bool state);
	void askTogglePomodoroTimer(bool state);
	void askToggleStayAwake(bool state);
	void askToggleAlwaysOnTop(bool state);
	void askSetPomodoroTime(int timeInSeconds);
	void askToggleIndicator(bool state);
	void askSetIndicatorAlignment(const QString& alignment);
	void askSetPreviewType(const QString& type);
	void devOpenLogs();
	void devDocument();
	void devDocumentCurrent();
	void devStylist();
	void devStylistStyleSheets();
	void devStylistUnstyle();

private:
	static constexpr char CHECK_BOX_EDITOR_THEME[] = "has_editor_theme";
	static constexpr char CHECK_BOX_WINDOW_THEME[] = "has_window_theme";
	static constexpr char GROUP_EDITOR_THEMES[] = "editor_themes";
	static constexpr char GROUP_WINDOW_THEMES[] = "window_themes";
	static constexpr char GROUP_WRAPS[] = "wrap_modes";
	static constexpr char SLIDER_TABS[] = "tab_stops";
	static constexpr char CHECK_BOX_LINE_HIGHLIGHT[] = "line_highlight";
	static constexpr char CHECK_BOX_LINE_NUMBERS[] = "line_number_area";
	static constexpr char CHECK_BOX_SHADOW[] = "editor_shadow";
	static constexpr char CHECK_BOX_BLINK[] = "cursor_blink";
	static constexpr char CHECK_BOX_BLOCK[] = "cursor_block";
	static constexpr char CHECK_BOX_CENTER_ON_SCROLL[] = "center_on_scroll";
	static constexpr char CHECK_BOX_ENSURE_VISIBLE[] = "ensure_visible";
	static constexpr char CHECK_BOX_TYPEWRITER[] = "typewriter";
	static constexpr char CHECK_BOX_LINE_POS[] = "line_position";
	static constexpr char CHECK_BOX_COL_POS[] = "column_position";
	static constexpr char CHECK_BOX_LINES[] = "line_count";
	static constexpr char CHECK_BOX_WORDS[] = "word_count";
	static constexpr char CHECK_BOX_CHARS[] = "character_count";
	static constexpr char CHECK_BOX_POMODORO[] = "pomodoro_timer";
	static constexpr char CHECK_BOX_STAY_AWAKE[] = "stay_awake";
	static constexpr char CHECK_BOX_ALWAYS_ON_TOP[] = "always_on_top";
	static constexpr char SLIDER_POMODORO[] = "pomodoro_times";
	static constexpr char CHECK_BOX_INDICATOR[] = "has_indicator";
	static constexpr char GROUP_INDICATOR_ALIGN[] = "indicator_alignments";
	static constexpr char GROUP_PREVIEW[] = "preview_types";
	static constexpr char QRC_EDITOR[] = ":/menu/themes/editor/";
	static constexpr char QRC_MAIN_WINDOW[] = ":/menu/themes/window/";

	const bool m_isDev;
	const StdFsPath m_userData;
	const StdFsPath m_userDocuments;
	std::map<QString, ActionGroup*> m_actionGroups;
	std::map<QString, int> m_sliderValues;
	std::map<QString, bool> m_checkBoxStates;

	void makeActionGroups();
	void makeBespokeActionGroups();
	void file();
	void project();
	void view();
	void help();
	void dev();
	void addActionsToBoxes(QComboBox* comboBox, ActionGroup* actionGroup);
	void addFontDialog(QMdiArea* multiDocArea);
	LiveFontDialog* fontDialog();
	QGroupBox* themesGroupBox();
	QGroupBox* fontGroupBox();
	QGroupBox* editorGroupBox();
	QGroupBox* cursorGroupBox();
	QGroupBox* meterGroupBox();
	QGroupBox* toolsGroupBox();
	QGroupBox* mixedGroupBox();

	QAction* selectedEditorTheme() const { return m_actionGroups.at(GROUP_EDITOR_THEMES)->checkedAction(); }
	QAction* selectedWindowTheme() const { return m_actionGroups.at(GROUP_WINDOW_THEMES)->checkedAction(); }
	QAction* selectedWrapMode() const { return m_actionGroups.at(GROUP_WRAPS)->checkedAction(); }
	QAction* selectedIndicatorAlignment() const { return m_actionGroups.at(GROUP_INDICATOR_ALIGN)->checkedAction(); }
	QAction* selectedPreviewType() const { return m_actionGroups.at(GROUP_PREVIEW)->checkedAction(); }

	void setGroupSelectedAction(ActionGroup* actionGroup, const StdFsPath& value)
	{
		for (auto i = 0; i < actionGroup->actions().count(); ++i) {
			auto action = actionGroup->actions().at(i);
			if (Path::toStdFs(action->data()) == value) {
				action->setChecked(true);
				return;
			}
		}
	}

	void setBespokeGroupSelectedAction(ActionGroup* actionGroup, const QString& value)
	{
		for (auto i = 0; i < actionGroup->actions().count(); ++i) {
			auto action = actionGroup->actions().at(i);
			if (action->data().toString() == value) {
				action->setChecked(true);
				return;
			}
		}
	}

private slots:
	void appearanceDialog();
};
