#pragma once

#include <QImage>
#include <QPainter>
#include <QPixmap>
#include <QRgb>

namespace Image
{
	inline QPixmap greyscale(const QPixmap& pixmap)
	{
		auto image = pixmap.toImage();
		QImage grayscaled_image = image;

		for (auto x = 0; x < image.width(); ++x)
			for (auto y = 0; y < image.height(); ++y) {
				auto pixel = image.pixel(x, y);
				auto grey = qGray(pixel);
				auto alpha = qAlpha(pixel);
				auto rgba = qRgba(grey, grey, grey, alpha);

				grayscaled_image.setPixel(x, y, rgba);
			}

		return QPixmap::fromImage(grayscaled_image);
	}

	inline QPixmap greyscale(const char* filePath)
	{
		return greyscale(QPixmap(filePath));
	}

	inline QPixmap opacify(const QPixmap& pixmap, double opacity)
	{
		QPixmap new_pixmap(pixmap.size());
		new_pixmap.fill(Qt::transparent);

		QPainter painter(&new_pixmap);
		painter.setOpacity(opacity);
		painter.drawPixmap(0, 0, pixmap);
		painter.end();

		return new_pixmap;
	}

	inline QPixmap opacify(const char* filePath, double opacity)
	{
		return opacify(QPixmap(filePath), opacity);
	}
}
