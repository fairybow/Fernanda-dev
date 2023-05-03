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

class MenuBar : public QMenuBar
{
	using StdFsPath = std::filesystem::path;

	Q_OBJECT

public:
	MenuBar(const char* name, QWidget* parent);

	void makeSubmenus();

signals:
	MenuBar::StdFsPath getUserDataPath();
	MenuBar::StdFsPath getResourcePaths();

private:
	std::map<QString, ActionGroup*> m_actionGroups;

	void makeActionGroups();
	void view();
	void help();
	void addActionsToBoxes(QComboBox* comboBox, ActionGroup* actionGroup);
	void addFontDialog(QMdiArea* multiDocArea);
	LiveFontDialog* fontDialog();
	const QFont initialFont();

private slots:
	void appearanceDialog();
};
