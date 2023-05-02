#include "Meter.h"

Meter::Meter(const char* name, QWidget* parent)
	: QWidget(parent)
{
	for (auto& widget : QWidgetList{ this, m_positions, m_separator, m_counts })
		widget->setObjectName(name);
	Layout::box({ m_positions, m_separator, m_counts, m_refresh }, this, Layout::Line::Horizontally);
	m_separator->setText("/");
	m_positions->setGraphicsEffect(opacify(0.8));
	m_separator->setGraphicsEffect(opacify(0.3));
	m_counts->setGraphicsEffect(opacify(0.8));
}

QGraphicsOpacityEffect* Meter::opacify(double qreal)
{
	auto effect = new QGraphicsOpacityEffect(this);
	effect->setOpacity(qreal);
	return effect;
}
