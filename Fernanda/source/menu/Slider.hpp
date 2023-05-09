#pragma once

#include <QSlider>

class Slider : public QSlider
{
public:
	Slider(Qt::Orientation orientation, QWidget* parent = nullptr, int min = 0, int max = 100)
		: QSlider(orientation, parent)
	{
		setRange(min, max);
	}

	void setRange(int bottom, int top)
	{
		setMinimum(bottom);
		setMaximum(top);
	}
};
