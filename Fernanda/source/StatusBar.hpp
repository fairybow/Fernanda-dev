#pragma once

#include <QStatusBar>

class StatusBar : public QStatusBar
{
public:
	inline StatusBar(const char* name, QWidget* parent)
		: QStatusBar(parent)
	{
		setObjectName(name);
	}
};
