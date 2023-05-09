#pragma once

#include <QFontMetrics>
#include <QSlider>
#include <QString>
#include <QStyleOptionSlider>

class Slider : public QSlider
{
public:
	Slider(Qt::Orientation orientation, QWidget* parent = nullptr, const QString& idleText = QString(), int min = 0, int max = 100)
		: QSlider(orientation, parent), m_idleText(idleText)
	{
		setRange(min, max);
	}

	void setRange(int bottom, int top)
	{
		setMinimum(bottom);
		setMaximum(top);
	}

protected:
	virtual void paintEvent(QPaintEvent* event)
	{
		QSlider::paintEvent(event);
		QPainter painter(this);
		painter.setPen(Qt::black);
		auto display_value = value();
		auto display = m_idleText + " " + QString::number(display_value);

		QRect slider_rect = rect();
		QFontMetrics font_metrics(font());
		auto text_width = font_metrics.horizontalAdvance(display);
		auto x = (slider_rect.width() - text_width) / 2;

		QStyleOptionSlider option;
		initStyleOption(&option);
		auto handle_rect = style()->subControlRect(QStyle::CC_Slider, &option, QStyle::SC_SliderHandle, this);
		auto text_height = font_metrics.height();
		auto y = (handle_rect.top() + text_height) / 2;

		painter.drawText(x, y, display);
	}

private:
	const QString m_idleText;
};
