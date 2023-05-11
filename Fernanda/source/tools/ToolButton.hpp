#pragma once

#include "../common/StatusBarButton.hpp"

#include <QMouseEvent>
#include <QString>

class ToolButton : public StatusBarButton
{
public:
	ToolButton(const QString& text, QWidget* parent = nullptr)
		: StatusBarButton("StatusBarButton", text, parent, 0.3)
	{
		setCheckable(true);
	}

public slots:
	virtual void setVisible(bool visible)
	{
		if (isChecked())
			setChecked(false);
		StatusBarButton::setVisible(visible);
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
