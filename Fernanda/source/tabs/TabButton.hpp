#pragma once

#include "../common/Svg.hpp"
#include "../common/Widget.hpp"

#include <QColor>
#include <QEnterEvent>
#include <QStyle>
#include <QToolButton>

class TabButton : public Widget<QToolButton>
{
	Q_OBJECT;
	Q_PROPERTY(QColor iconColor READ iconColor WRITE setIconColor)

public:
	TabButton(const char* name, Svg::Ui icon, QWidget* parent = nullptr,
		Svg::Ui flag = Svg::Ui{}, double iconScale = 1.0, double flagScale = 1.0)
		: Widget(name, parent),
		m_icon(icon),
		m_flag(flag),
		m_iconScale(iconScale),
		m_flagScale(flagScale)
	{
		style()->unpolish(this);
		style()->polish(this);
		updateIcon();
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
	virtual void changeEvent(QEvent* event) override
	{
		if (event->type() == QEvent::StyleChange)
			updateIcon();
		QToolButton::changeEvent(event);
	}

	virtual void enterEvent(QEnterEvent* event) override
	{
		if (m_flag != Svg::Ui{}) {
			m_hoveredOver = true;
			updateIcon();
		}
		QToolButton::enterEvent(event);
	}

	virtual void leaveEvent(QEvent* event) override
	{
		if (m_flag != Svg::Ui{}) {
			m_hoveredOver = false;
			updateIcon();
		}
		QToolButton::leaveEvent(event);
	}

private:
	Svg::Ui m_icon;
	Svg::Ui m_flag;
	double m_iconScale;
	double m_flagScale;
	QColor m_iconColor = Qt::black;
	bool m_flagged = false;
	bool m_hoveredOver = false;

	void updateIcon()
	{
		setIcon((m_flagged && !m_hoveredOver)
			? Svg::ui(m_flag, iconColor(), m_flagScale)
			: Svg::ui(m_icon, iconColor(), m_iconScale));
	}
};
