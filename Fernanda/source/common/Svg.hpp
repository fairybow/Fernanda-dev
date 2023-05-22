#pragma once

#include <QBuffer>
#include <QByteArray>
#include <QColor>
#include <QDomDocument>
#include <QFile>
#include <QIcon>
#include <QRegularExpression>
#include <QSize>
#include <QString>
#include <QSvgGenerator>
#include <QSvgRenderer>
#include <QTextStream>
#include <QPainter>
#include <QPixmap>

#include <map>

namespace Svg
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

		QByteArray adjustColor(const QString& fileName, const QColor& color)
		{
			QFile file(fileName);
			QDomDocument document;
			if (!file.open(QIODevice::ReadOnly | QIODevice::Text)
				|| !document.setContent(&file))
				return QByteArray();

			auto adjust_element_color = [&](const QString& tagName, const QString& attributeName, const QString& valueName) {


				auto nodes = document.elementsByTagName(tagName);
				for (auto i = 0; i < nodes.count(); ++i) {
					auto element = nodes.at(i).toElement();
					auto style = element.attribute(attributeName);

					style.replace(QRegularExpression(QString("%1:[^;]+").arg(valueName)), QString("%1:").arg(valueName) + color.name()); // handle multiple

					element.setAttribute(attributeName, style);
				}
			};

			adjust_element_color("line", "style", "stroke");
			adjust_element_color("polyline", "style", "stroke");

			QBuffer buffer;
			buffer.open(QIODevice::WriteOnly);
			QTextStream stream(&buffer);
			document.save(stream, 0);
			return buffer.data();
		}

		inline QIcon icon(const QString& fileName, const QColor& color = Qt::black, double scale = 1.0)
		{
			auto data = adjustColor(fileName, color);
			QSvgRenderer renderer(data);
			QPixmap pixmap(QSize(18, 18) *= scale);
			pixmap.fill(Qt::transparent);
			QPainter painter(&pixmap);
			renderer.render(&painter);
			return QIcon(pixmap);
		}
	}

	inline QIcon ui(Ui name, const QColor& color = Qt::black, double scale = 1.0)
	{
		auto it = uiPaths.find(name);
		if (it != uiPaths.end())
			return icon(it->second, color, scale);
		return QIcon();
	}
}
