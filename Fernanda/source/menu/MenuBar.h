#pragma once

#include "../common/Layout.hpp"
#include "ActionGroup.hpp"
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

constexpr char EDITOR_THEMES[] = "editor_themes";
constexpr char WINDOW_THEMES[] = "window_themes";

class MenuBar : public QMenuBar
{
	using StdFsPath = std::filesystem::path;

	Q_OBJECT

public:
	MenuBar(const char* name, bool isDev = false, QWidget* parent = nullptr);

	void makeSubmenus();

signals:
	MenuBar::StdFsPath getUserDataPath();
	void askStyleEditor(StdFsPath path);
	void askStyleWindow(StdFsPath path);

private:
	std::map<QString, ActionGroup*> m_actionGroups;
	const bool m_isDev;

	void makeActionGroups();
	void view();
	void help();
	void addActionsToBoxes(QComboBox* comboBox, ActionGroup* actionGroup);
	void addFontDialog(QMdiArea* multiDocArea);
	LiveFontDialog* fontDialog();
	const QFont initialFont();

	inline QAction* selectedEditorTheme() const { return m_actionGroups.at(EDITOR_THEMES)->checkedAction(); }
	inline QAction* selectedWindowTheme() const { return m_actionGroups.at(WINDOW_THEMES)->checkedAction(); }

private slots:
	void appearanceDialog();
};
