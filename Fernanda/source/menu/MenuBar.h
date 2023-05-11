#pragma once

#include "../common/Emoji.hpp"
#include "../common/Layout.hpp"
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

constexpr char GROUP_EDITOR_THEMES[] = "editor_themes";
constexpr char GROUP_WINDOW_THEMES[] = "window_themes";
constexpr char GROUP_WRAPS[] = "wrap_modes";
constexpr char GROUP_INDICATOR_POS[] = "indicator_alignments";
constexpr char GROUP_PREVIEW[] = "preview_types";
constexpr char SLIDER_TABS[] = "tab_stops";
constexpr char SLIDER_POMODORO[] = "pomodoro_times";
constexpr char QRC_EDITOR[] = ":/menu/themes/editor/";
constexpr char QRC_MAIN_WINDOW[] = ":/menu/themes/window/";

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
	void setSelectedWrapMode(const QString& mode) { setGroupSelectedAction(m_actionGroups[GROUP_WRAPS], mode); }
	void setSelectedIndicatorPosition(const QString& position) { setGroupSelectedAction(m_actionGroups[GROUP_INDICATOR_POS], position); }
	void setSelectedPreviewType(const QString& type) { setGroupSelectedAction(m_actionGroups[GROUP_PREVIEW], type); }
	void setSelectedPomodoroTime(int timeInSeconds) { m_sliderValues[SLIDER_POMODORO] = timeInSeconds; }

signals:
	MenuBar::StdFsPath getUserDataPath();
	QFont getUserFont();
	void askStyleEditor(StdFsPath path);
	void askStyleWindow(StdFsPath path);
	void askChangeFont(const QFont& font);
	void askSetTabStop(int pixels);
	void askSetWrapMode(const QString& mode);
	void askSetIndicatorPosition(const QString& position);
	void askSetPreviewType(const QString& type);
	void askSetPomodoroTime(int timeInSeconds);

private:
	std::map<QString, ActionGroup*> m_actionGroups;
	std::map<QString, int> m_sliderValues;
	const bool m_isDev;

	void makeActionGroups();
	void makeBespokeActionGroups();
	void view();
	void help();
	void addActionsToBoxes(QComboBox* comboBox, ActionGroup* actionGroup);
	void addFontDialog(QMdiArea* multiDocArea);
	LiveFontDialog* fontDialog();

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

	void setGroupSelectedAction(ActionGroup* actionGroup, const QString& value)
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
