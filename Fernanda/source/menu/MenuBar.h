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
constexpr char QRC_EDITOR[] = ":/menu/themes/editor/";
constexpr char QRC_MAIN_WINDOW[] = ":/menu/themes/window/";

class MenuBar : public Widget<QMenuBar>
{
	using StdFsPath = std::filesystem::path;

	Q_OBJECT

public:
	MenuBar(const char* name, bool isDev = false, QWidget* parent = nullptr);

	void makeSubmenus();

	StdFsPath defaultEditorTheme() const { return Path::toStdFs(QRC_EDITOR) / "Snooze.fernanda_editor"; };
	StdFsPath defaultWindowTheme() const { return Path::toStdFs(QRC_MAIN_WINDOW) / "Light.fernanda_window"; };

	void setSelectedEditorTheme(const StdFsPath& path) { setSelectedGroupAction(m_actionGroups[EDITOR_THEMES], path); };
	void setSelectedWindowTheme(const StdFsPath& path) { setSelectedGroupAction(m_actionGroups[WINDOW_THEMES], path); };

signals:
	MenuBar::StdFsPath getUserDataPath();
	QFont getUserFont();
	void askStyleEditor(StdFsPath path);
	void askStyleWindow(StdFsPath path);
	void askChangeFont(const QFont& font);

private:
	std::map<QString, ActionGroup*> m_actionGroups;
	const bool m_isDev;

	void makeActionGroups();
	void setSelectedGroupAction(ActionGroup* actionGroup, const StdFsPath& path);
	void view();
	void help();
	void addActionsToBoxes(QComboBox* comboBox, ActionGroup* actionGroup);
	void addFontDialog(QMdiArea* multiDocArea);
	LiveFontDialog* fontDialog();

	QAction* selectedEditorTheme() const { return m_actionGroups.at(EDITOR_THEMES)->checkedAction(); }
	QAction* selectedWindowTheme() const { return m_actionGroups.at(WINDOW_THEMES)->checkedAction(); }

private slots:
	void appearanceDialog();
};
