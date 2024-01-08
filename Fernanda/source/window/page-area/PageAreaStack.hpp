#pragma once

#include <QStackedWidget>

class PageAreaStack : public QStackedWidget
{
	Q_OBJECT

public:
	PageAreaStack(QWidget* parent = nullptr)
		: QStackedWidget(parent)
	{}

	~PageAreaStack()
	{
		qDebug() << __FUNCTION__;
	}
};
