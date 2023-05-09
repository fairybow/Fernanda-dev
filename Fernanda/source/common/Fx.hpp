#pragma once

#include <QGraphicsBlurEffect>
#include <QGraphicsOpacityEffect>

namespace Fx
{
	inline QGraphicsBlurEffect* blur(int radius = 5, QObject* parent = nullptr)
	{
		auto effect = new QGraphicsBlurEffect(parent);
		effect->setBlurHints(QGraphicsBlurEffect::QualityHint);
		effect->setBlurRadius(radius);
		return effect;
	}

	inline QGraphicsOpacityEffect* opacify(double opacity = 0.5, QObject* parent = nullptr)
	{
		auto effect = new QGraphicsOpacityEffect(parent);
		effect->setOpacity(opacity); 
		return effect;
	}
}
