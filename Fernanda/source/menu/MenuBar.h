#pragma once

#include "LiveFontDialog.hpp"
#include "MenuPopup.hpp"

#include <QMenuBar>

class MenuBar : public QMenuBar
{
public:
	MenuBar(QWidget* parent);

	void makeSubmenus();

private:
	void help();
};
