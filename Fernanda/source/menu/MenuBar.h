#pragma once

#include "../common/Layout.hpp"
#include "LiveFontDialog.hpp"
#include "MenuPopup.hpp"
#include "Resource.hpp"

#include <QActionGroup>
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
public:
	MenuBar(const char* name, QWidget* parent);

	void makeSubmenus();

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
