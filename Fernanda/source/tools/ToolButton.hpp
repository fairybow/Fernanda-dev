#pragma once

#include "../common/UiButton.hpp"

#include <QChar>
#include <QMouseEvent>
#include <QString>

class ToolButton : public UiButton
{
public:
	ToolButton(const QChar& icon, QWidget* parent = nullptr)
		: UiButton("ToolButton", icon, parent)
	{
		setCheckable(true);

		// Fix highlighting issue:
		// even though opacity should reverse,
		// highlight should remain while hovered;
		// address like TabButton

	}

public slots:
	virtual void setVisible(bool visible)
	{
		if (isChecked())
			setChecked(false);
		UiButton::setVisible(visible);
	}

protected:
	virtual void mousePressEvent(QMouseEvent* event) override
	{
		if (event->button() == Qt::RightButton) {
			setChecked(!isChecked());
			return;
		}

		UiButton::mousePressEvent(event);
	}
};
