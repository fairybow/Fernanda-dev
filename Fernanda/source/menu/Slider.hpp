#pragma once

#include "../common/Layout.hpp"

#include <QLabel>
#include <QSlider>
#include <QString>
#include <QVector>

#include <optional>

class Slider : public QWidget
{
public:
	Slider(Qt::Orientation orientation, QWidget* parent = nullptr, const QString& label = QString(),
		bool hasDisplay = true, int min = 0, int max = 100)
		: QWidget(parent), m_slider(new QSlider(orientation, this))
	{
		// set starting value/valueDisplay
		// spacing

		setRange(min, max);

		if (!label.isEmpty())
			m_label = new QLabel(label, this);

		if (hasDisplay) {
			m_valueDisplay = new QLabel(this);
			connect(m_slider, &QSlider::valueChanged, this, [&](int value) {
				m_valueDisplay.value()->setText(QString::number(value));
				});
		}

		QWidgetList widgets;

		if (m_label.has_value()) widgets.append(m_label.value());
		widgets << m_slider;
		if (m_valueDisplay.has_value()) widgets.append(m_valueDisplay.value());

		Layout::box(widgets, this, Layout::Line::Horizontally);
	}

	static void setRange(QSlider* slider, int bottom, int top)
	{
		slider->setMinimum(bottom);
		slider->setMaximum(top);
	}

	void setRange(int bottom, int top)
	{
		setRange(m_slider, bottom, top);
	}

private:
	QSlider* m_slider;
	std::optional<QLabel*> m_label;
	std::optional<QLabel*> m_valueDisplay;
};
