#pragma once

#include "../common/StatusBarButton.hpp"

#include <QMouseEvent>
#include <QString>

class Tool : public StatusBarButton
{
public:
	Tool(const QString& text, QWidget* parent = nullptr)
		: StatusBarButton("StatusBarButton", text, parent, 0.3)
	{
		setCheckable(true);
	}

	void toggle(bool value)
	{
		if (isChecked())
			setChecked(false);
		setVisible(value);
	}

protected:
	virtual void mousePressEvent(QMouseEvent* event) override
	{
		if (event->button() == Qt::RightButton) {
			setChecked(!isChecked());
			return;
		}
		QPushButton::mousePressEvent(event);
	}
};
