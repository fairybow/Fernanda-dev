#pragma once

#include "../common/Emoji.hpp"
#include "../common/Layout.hpp"
#include "../common/Utility.hpp"
#include "../common/Widget.hpp"
#include "ActionGroup.h"
#include "ComboBox.hpp"
#include "LiveFontDialog.hpp"
#include "Popup.hpp"
#include "Slider.hpp"

#include <QCheckBox>
#include <QDesktopServices>
#include <QFileDialog>
#include <QGroupBox>
#include <QMenuBar>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QVector>
#include <QWidgetAction>

#include <map>
#include <filesystem>

class MenuBar : public Widget<QMenuBar>
{
	using StdFsPath = std::filesystem::path;

	Q_OBJECT

public:
	MenuBar(const char* name, bool isDev = false, QWidget* parent = nullptr);

	void makeSubmenus();

	StdFsPath defaultEditorTheme() const { return Path::toStdFs(QRC_EDITOR) / "Snooze.fernanda_editor"; } // Set in Editor?
	StdFsPath defaultWindowTheme() const { return Path::toStdFs(QRC_MAIN_WINDOW) / "Light.fernanda_window"; } // Set in MW?

	void setSelectedEditorTheme(const StdFsPath& path) { setGroupSelectedAction(m_actionGroups[GROUP_EDITOR_THEMES], path); }
	void setSelectedWindowTheme(const StdFsPath& path) { setGroupSelectedAction(m_actionGroups[GROUP_WINDOW_THEMES], path); }
	void setSelectedTabStop(int pixels) { m_sliderValues[SLIDER_TABS] = pixels; }
	void setSelectedWrapMode(const QString& mode) { setBespokeGroupSelectedAction(m_actionGroups[GROUP_WRAPS], mode); }
	void setCheckBoxLineHighlight(bool state) { m_checkBoxStates[CHECK_BOX_LINE_HIGHLIGHT] = state; }
	void setCheckBoxLineNumbers(bool state) { m_checkBoxStates[CHECK_BOX_LINE_NUMBERS] = state; }
	void setCheckBoxShadow(bool state) { m_checkBoxStates[CHECK_BOX_SHADOW] = state; }

	/*bool m_hasCursorBlink = true;
	bool m_hasCursorBlock = true;
	bool m_hasCursorCenterOnScroll = true;
	bool m_hasCursorEnsureVisible = true;
	bool m_hasCursorTypewriter = true;
	*/

	//void setSelectedIndicatorPosition(const QString& position) { setBespokeGroupSelectedAction(m_actionGroups[GROUP_INDICATOR_POS], position); }
	//void setSelectedPreviewType(const QString& type) { setBespokeGroupSelectedAction(m_actionGroups[GROUP_PREVIEW], type); }
	void setCheckBoxLinePosition(bool state) { m_checkBoxStates[CHECK_BOX_LINE_POS] = state; }
	void setCheckBoxColumnPosition(bool state) { m_checkBoxStates[CHECK_BOX_COL_POS] = state; }
	void setCheckBoxLineCount(bool state) { m_checkBoxStates[CHECK_BOX_LINES] = state; }
	void setCheckBoxWordCount(bool state) { m_checkBoxStates[CHECK_BOX_WORDS] = state; }
	void setCheckBoxCharacterCount(bool state) { m_checkBoxStates[CHECK_BOX_CHARS] = state; }
	void setCheckBoxPomodoroTimer(bool state) { m_checkBoxStates[CHECK_BOX_POMODORO] = state; }
	void setCheckBoxStayAwake(bool state) { m_checkBoxStates[CHECK_BOX_STAY_AWAKE] = state; }
	void setCheckBoxAlwaysOnTop(bool state) { m_checkBoxStates[CHECK_BOX_ALWAYS_ON_TOP] = state; }
	void setSelectedPomodoroTime(int timeInSeconds) { m_sliderValues[SLIDER_POMODORO] = timeInSeconds; }

signals:
	MenuBar::StdFsPath getUserDataPath();
	QFont getUserFont();
	void askStyleEditor(StdFsPath path);
	void askStyleWindow(StdFsPath path);
	void askChangeFont(const QFont& font);
	void askSetTabStop(int pixels);
	void askSetWrapMode(const QString& mode);
	void askToggleLineHighlight(bool state);
	void askToggleLineNumbers(bool state);
	void askToggleShadow(bool state);
	void askSetIndicatorPosition(const QString& position);
	void askSetPreviewType(const QString& type);
	void askToggleLinePosition(bool state);
	void askToggleColumnPosition(bool state);
	void askToggleLineCount(bool state);
	void askToggleWordCount(bool state);
	void askToggleCharacterCount(bool state);
	void askTogglePomodoroTimer(bool state);
	void askToggleStayAwake(bool state);
	void askToggleAlwaysOnTop(bool state);
	void askSetPomodoroTime(int timeInSeconds);

private:
	static constexpr char GROUP_EDITOR_THEMES[] = "editor_themes";
	static constexpr char GROUP_WINDOW_THEMES[] = "window_themes";

	//
	static constexpr char GROUP_WRAPS[] = "wrap_modes";
	static constexpr char CHECK_BOX_LINE_HIGHLIGHT[] = "line_highlight";
	static constexpr char CHECK_BOX_LINE_NUMBERS[] = "line_number_area";
	static constexpr char CHECK_BOX_SHADOW[] = "editor_shadow";
	//

	static constexpr char GROUP_INDICATOR_POS[] = "indicator_alignments";
	static constexpr char GROUP_PREVIEW[] = "preview_types";
	static constexpr char SLIDER_TABS[] = "tab_stops";
	static constexpr char SLIDER_POMODORO[] = "pomodoro_times";
	static constexpr char CHECK_BOX_LINE_POS[] = "line_position";
	static constexpr char CHECK_BOX_COL_POS[] = "column_position";
	static constexpr char CHECK_BOX_LINES[] = "line_count";
	static constexpr char CHECK_BOX_WORDS[] = "word_count";
	static constexpr char CHECK_BOX_CHARS[] = "character_count";
	static constexpr char CHECK_BOX_POMODORO[] = "pomodoro_timer";
	static constexpr char CHECK_BOX_STAY_AWAKE[] = "stay_awake";
	static constexpr char CHECK_BOX_ALWAYS_ON_TOP[] = "always_on_top";
	static constexpr char QRC_EDITOR[] = ":/menu/themes/editor/";
	static constexpr char QRC_MAIN_WINDOW[] = ":/menu/themes/window/";

	std::map<QString, ActionGroup*> m_actionGroups;
	std::map<QString, int> m_sliderValues;
	std::map<QString, bool> m_checkBoxStates;
	const bool m_isDev;

	void makeActionGroups();
	void makeBespokeActionGroups();
	void view();
	void help();
	void addActionsToBoxes(QComboBox* comboBox, ActionGroup* actionGroup);
	void addFontDialog(QMdiArea* multiDocArea);
	LiveFontDialog* fontDialog();
	QGroupBox* themesGroupBox();
	QGroupBox* fontGroupBox();
	QGroupBox* editorGroupBox();
	QGroupBox* cursorGroupBox();
	QGroupBox* meterGroupBox();
	QGroupBox* toolsGroupBox();

	QAction* selectedEditorTheme() const { return m_actionGroups.at(GROUP_EDITOR_THEMES)->checkedAction(); }
	QAction* selectedWindowTheme() const { return m_actionGroups.at(GROUP_WINDOW_THEMES)->checkedAction(); }
	QAction* selectedWrapMode() const { return m_actionGroups.at(GROUP_WRAPS)->checkedAction(); }
	QAction* selectedIndicatorPosition() const { return m_actionGroups.at(GROUP_INDICATOR_POS)->checkedAction(); }
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
