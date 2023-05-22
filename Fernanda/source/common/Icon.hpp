#pragma once

#include <QIcon>
#include <QSize>
#include <QString>
#include <QSvgRenderer>
#include <QPainter>
#include <QPixmap>

#include <map>

namespace Icon
{
	enum class Ui {
		Add,
		ChevronBack,
		ChevronDown,
		ChevronForward,
		ChevronUp,
		Close,
		Ellipse
	};

	namespace
	{
		const std::map<Ui, QString> uiPaths = {
		{ Ui::Add, ":/ui/add-outline.svg" },
		{ Ui::ChevronBack, ":/ui/chevron-back-outline.svg" },
		{ Ui::ChevronDown, ":/ui/chevron-down-outline.svg" },
		{ Ui::ChevronForward, ":/ui/chevron-forward-outline.svg" },
		{ Ui::ChevronUp, ":/ui/chevron-up-outline.svg" },
		{ Ui::Close, ":/ui/close-outline.svg" },
		{ Ui::Ellipse, ":/ui/ellipse.svg" }
		};

		inline QIcon icon(const QString& fileName, double scale = 1.0)
		{
			QSvgRenderer renderer(fileName);
			auto size = QSize(18, 18);
			QPixmap pixmap(size *= scale);
			pixmap.fill(Qt::transparent);
			QPainter painter(&pixmap);
			renderer.render(&painter);
			return QIcon(pixmap);
		}
	}

	inline QIcon ui(Ui name, double scale = 1.0)
	{
		auto it = uiPaths.find(name);
		if (it != uiPaths.end())
			return icon(it->second, scale);
		return QIcon();
	}
}
