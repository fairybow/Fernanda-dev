#include "Meter.h"

Meter::Meter(QWidget* parent)
	: QWidget(parent)
{
	//setObjectName(name);
	//for (auto& widget : { m_positions, m_separator, m_counts })
		//widget->setObjectName(name);
	Layout::box(this, { m_positions, m_separator, m_counts, m_refresh }, Layout::Line::Horizontally);
	m_separator->setText(QStringLiteral("/"));
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
