#pragma once

#include <QStatusBar>

class StatusBar : public QStatusBar
{
public:
	inline StatusBar(const char* name, QWidget* parent = nullptr)
		: QStatusBar(parent)
	{
		setObjectName(name);
		setMaximumHeight(22);
	}
};
