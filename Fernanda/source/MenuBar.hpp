#pragma once

#include "common/HtmlString.hpp"

#include <QMenuBar>

class MenuBar : public QMenuBar
{
public:
	inline MenuBar(const char* name, QWidget* parent)
		: QMenuBar(parent)
	{
		setObjectName(name);
	}
};
