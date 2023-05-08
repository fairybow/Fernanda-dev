#pragma once

#include "../common/Layout.hpp"
#include "../common/Widget.hpp"
#include "ActionGroup.h"
#include "LiveFontDialog.hpp"
#include "Popup.hpp"

#include <QComboBox>
#include <QDesktopServices>
#include <QFileDialog>
#include <QMenuBar>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QVector>

#include <map>
#include <filesystem>

constexpr char EDITOR_THEMES[] = "editor_themes";
constexpr char WINDOW_THEMES[] = "window_themes";
constexpr char TABS[] = "tab_stops";
constexpr char WRAPS[] = "wrap_modes";
constexpr char INDICATOR_POS[] = "indicator_alignments";
constexpr char PREVIEW[] = "preview_types";
constexpr char POMODORO[] = "pomodoro_times";
constexpr char QRC_EDITOR[] = ":/menu/themes/editor/";
constexpr char QRC_MAIN_WINDOW[] = ":/menu/themes/window/";

class MenuBar : public Widget<QMenuBar>
{
	using StdFsPath = std::filesystem::path;

	Q_OBJECT

public:
	MenuBar(const char* name, bool isDev = false, QWidget* parent = nullptr);

	void makeSubmenus();

	StdFsPath defaultEditorTheme() const { return Path::toStdFs(QRC_EDITOR) / "Snooze.fernanda_editor"; }; // Set in Editor?
	StdFsPath defaultWindowTheme() const { return Path::toStdFs(QRC_MAIN_WINDOW) / "Light.fernanda_window"; }; // Set in MW?

	void setSelectedEditorTheme(const StdFsPath& path) { setGroupSelectedAction(m_actionGroups[EDITOR_THEMES], path); };
	void setSelectedWindowTheme(const StdFsPath& path) { setGroupSelectedAction(m_actionGroups[WINDOW_THEMES], path); };
	void setSelectedTabStop(int pixels) { setGroupSelectedAction(m_actionGroups[TABS], pixels); }; // switch to m_sliderValues
	void setSelectedWrapMode(const QString& mode) { setGroupSelectedAction(m_actionGroups[WRAPS], mode); };
	void setSelectedIndicatorPosition(const QString& position) { setGroupSelectedAction(m_actionGroups[INDICATOR_POS], position); };
	void setSelectedPreviewType(const QString& type) { setGroupSelectedAction(m_actionGroups[PREVIEW], type); };
	void setSelectedPomodoroTime(int timeInSeconds) { setGroupSelectedAction(m_actionGroups[POMODORO], timeInSeconds); }; // switch to m_sliderValues

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
	//void setSelectedGroupAction(ActionGroup* actionGroup, const StdFsPath& path);
	void view();
	void help();
	void addActionsToBoxes(QComboBox* comboBox, ActionGroup* actionGroup);
	void addFontDialog(QMdiArea* multiDocArea);
	LiveFontDialog* fontDialog();

	QAction* selectedEditorTheme() const { return m_actionGroups.at(EDITOR_THEMES)->checkedAction(); }
	QAction* selectedWindowTheme() const { return m_actionGroups.at(WINDOW_THEMES)->checkedAction(); }
	QAction* selectedTabStop() const { return m_actionGroups.at(TABS)->checkedAction(); } // switch to m_sliderValues
	QAction* selectedWrapMode() const { return m_actionGroups.at(WRAPS)->checkedAction(); }
	QAction* selectedIndicatorPosition() const { return m_actionGroups.at(INDICATOR_POS)->checkedAction(); }
	QAction* selectedPreviewType() const { return m_actionGroups.at(PREVIEW)->checkedAction(); }
	QAction* selectedPomodoroTime() const { return m_actionGroups.at(POMODORO)->checkedAction(); } // switch to m_sliderValues

	template<typename T>
	void setGroupSelectedAction(ActionGroup* actionGroup, T value)
	{
		for (auto i = 0; i < actionGroup->actions().count(); ++i) {
			auto action = actionGroup->actions().at(i);
			if (action->data().value<T>() == value) {
				action->setChecked(true);
				return;
			}
		}
	}

private slots:
	void appearanceDialog();
};
