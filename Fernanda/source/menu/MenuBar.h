#pragma once

#include "HtmlString.hpp"
#include "LiveFontDialog.hpp"

#include <QMenuBar>

class MenuBar : public QMenuBar
{
public:
	MenuBar(const char* name, QWidget* parent);

	void makeSubmenus();

private:
	void help();
};
