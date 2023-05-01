#pragma once

#include "../common/Layout.hpp"
#include "LiveFontDialog.hpp"
#include "MenuPopup.hpp"

#include <QActionGroup>
#include <QComboBox>
#include <QDesktopServices>
#include <QFileDialog>
#include <QFontDialog>
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

private slots:
	void appearanceDialog();
};
