#pragma once

#include "../common/Svg.hpp"

#include <QColor>
#include <QEnterEvent>
#include <QToolButton>

class TabButton : public Widget<QToolButton>
{
	Q_OBJECT;
	Q_PROPERTY(QColor iconColor READ iconColor WRITE setIconColor)

public:
	TabButton(const char* name, QWidget* parent = nullptr) // 2 scale args for icon and flag separately
		: Widget(name, parent)
	{
		updateIcon();

		//
		// subclass for CloseButton, AddButton, Scrolls
		//

	}

	bool flagged() const { return m_flagged; }
	QColor iconColor() const { return m_iconColor; }
	void setIconColor(const QColor& color) { m_iconColor = color; }

	void setFlagged(bool flagged)
	{
		m_flagged = flagged;
		updateIcon();
	}

protected:
	virtual void enterEvent(QEnterEvent* event) override
	{
		m_hoveredOver = true;
		updateIcon();
		QToolButton::enterEvent(event);
	}

	virtual void leaveEvent(QEvent* event) override
	{
		m_hoveredOver = false;
		updateIcon();
		QToolButton::leaveEvent(event);
	}

private:
	Svg::Ui m_icon;
	Svg::Ui m_flag;
	QColor m_iconColor = Qt::black;
	bool m_flagged = false;
	bool m_hoveredOver = false;

	void updateIcon()
	{
		setIcon((m_flagged && !m_hoveredOver) ? Svg::ui(m_flag, iconColor()) : Svg::ui(m_icon, iconColor()));
	}
};
