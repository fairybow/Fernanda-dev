#pragma once

#include "common/Fx.hpp"
#include "common/Layout.hpp"
#include "common/Widget.hpp"
#include "common/StatusBarButton.hpp"

#include <QLabel>
#include <QRegularExpression>

class Meter : public Widget<>
{
public:
	inline Meter(const char* name, QWidget* parent = nullptr)
		: Widget(name, parent)
	{
		for (auto& widget : QWidgetList{ m_positions, m_separator, m_counts })
			widget->setObjectName(name);
		setupLabels();
		Layout::box({ m_positions, m_separator, m_counts, m_refresh }, this, Layout::Line::Horizontally);
		
	}

private:
	QLabel* m_positions = new QLabel(this);
	QLabel* m_counts = new QLabel(this);
	QLabel* m_separator = new QLabel(this);
	StatusBarButton* m_refresh = new StatusBarButton("StatusBarButton", "\U0001F504", this);

	inline void setupLabels()
	{
		m_separator->setText("/");
		m_positions->setGraphicsEffect(
			Fx::opacify(0.8, m_positions));
		m_separator->setGraphicsEffect(
			Fx::opacify(0.3, m_separator));
		m_counts->setGraphicsEffect(
			Fx::opacify(0.8, m_counts));
	}
};
