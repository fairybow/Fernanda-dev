#pragma once

#include <QGraphicsBlurEffect>
#include <QGraphicsOpacityEffect>

namespace Fx
{
	inline QGraphicsBlurEffect* blur(int radius = 15, QObject* parent = nullptr)
	{
		auto effect = new QGraphicsBlurEffect(parent);
		effect->setBlurHints(QGraphicsBlurEffect::QualityHint);
		effect->setBlurRadius(radius);
		return effect;
	}

	inline QGraphicsOpacityEffect* opacify(double qreal, QObject* parent = nullptr)
	{
		auto effect = new QGraphicsOpacityEffect(parent);
		effect->setOpacity(qreal);
		return effect;
	}
}
