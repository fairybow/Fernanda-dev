#pragma once

#include "common/Layout.hpp"
#include "common/StatusBarButton.hpp"

#include <QLabel>
#include <QRegularExpression>

class Meter : public QWidget
{
public:
	inline Meter(const char* name, QWidget* parent)
		: QWidget(parent)
	{
		setObjectName(name);
	}
};
