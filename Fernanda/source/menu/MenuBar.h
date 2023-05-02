#pragma once

#include "../common/Layout.hpp"
#include "LiveFontDialog.hpp"
#include "MenuPopup.hpp"
#include "ResourceGroups.hpp"

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
	std::map<QString, QActionGroup*> m_actionGroups;

	void view();
	void help();
	void addActionsToBoxes(QComboBox* comboBox, QActionGroup* actionGroup);
	void addFontDialog(QMdiArea* multiDocArea);
	LiveFontDialog* fontDialog();
	const QFont initialFont();

private slots:
	void appearanceDialog();
};
