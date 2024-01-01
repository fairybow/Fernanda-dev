#pragma once

#include <QGraphicsBlurEffect>
#include <QGraphicsOpacityEffect>
#include <QWidget>

namespace Fx
{
	inline QGraphicsBlurEffect* blur(QWidget* widget, int radius = 5)
	{
		auto effect = new QGraphicsBlurEffect(widget);
		effect->setBlurHints(QGraphicsBlurEffect::QualityHint);
		effect->setBlurRadius(radius);

		widget->setGraphicsEffect(effect);

		return effect;
	}

	inline QGraphicsOpacityEffect* opacify(QWidget* widget, double opacity = 0.5)
	{
		auto effect = new QGraphicsOpacityEffect(widget);
		effect->setOpacity(opacity);

		widget->setGraphicsEffect(effect);

		return effect;
	}
}
