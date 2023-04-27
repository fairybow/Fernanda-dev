#pragma once

#include "common/Layout.hpp"
#include "common/StatusBarButton.hpp"

#include <QGraphicsOpacityEffect>
#include <QLabel>
#include <QRegularExpression>

class Meter : public QWidget
{
public:
	Meter(const char* name, QWidget* parent = nullptr);

private:
	QLabel* m_positions = new QLabel(this);
	QLabel* m_counts = new QLabel(this);
	QLabel* m_separator = new QLabel(this);
	StatusBarButton* m_refresh = new StatusBarButton("StatusBarButton", "\U0001F504", this);

	QGraphicsOpacityEffect* opacify(double qreal);
};
