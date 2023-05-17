#pragma once

#include <QTabBar>

class TrueTabBar : public QTabBar
{
	Q_OBJECT

public:
	TrueTabBar(QWidget* parent = nullptr)
		: QTabBar(parent)
	{
		//
	}
};
