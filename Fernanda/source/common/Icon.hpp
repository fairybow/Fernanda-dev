#pragma once

#include <QIcon>
#include <QString>
#include <QSvgRenderer>
#include <QPainter>
#include <QPixmap>

namespace Icon
{
	namespace
	{
		inline QIcon icon(const QString& fileName)
		{
			QSvgRenderer renderer(fileName);
			QPixmap pixmap(renderer.defaultSize());
			pixmap.fill(Qt::transparent);
			QPainter painter(&pixmap);
			renderer.render(&painter);
			return QIcon(pixmap);
		}
	}

	inline QIcon add()
	{
		return icon(":/ui/add-outline.svg");
	}

	inline QIcon chevronBack()
	{
		return icon(":/ui/chevron-back-outline.svg");
	}

	inline QIcon chevronDown()
	{
		return icon(":/ui/chevron-down-outline.svg");
	}

	inline QIcon chevronForward()
	{
		return icon(":/ui/chevron-forward-outline.svg");
	}

	inline QIcon chevronUp()
	{
		return icon(":/ui/chevron-up-outline.svg");
	}

	inline QIcon close()
	{
		return icon(":/ui/close-outline.svg");
	}

	inline QIcon ellipse()
	{
		return icon(":/ui/ellipse.svg");
	}
}
