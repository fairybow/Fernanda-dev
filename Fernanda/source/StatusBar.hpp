#pragma once

#include <QStatusBar>

class StatusBar : public QStatusBar
{
public:
	inline StatusBar(QWidget* parent)
		: QStatusBar(parent)
	{
		//setObjectName(name);
		setMaximumHeight(22);
	}
};
