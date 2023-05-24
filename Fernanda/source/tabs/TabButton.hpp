#pragma once

#include "../common/Icon.hpp"
#include "../common/Widget.hpp"

#include <QEnterEvent>
#include <QToolButton>

class TabButton : public Widget<QToolButton>
{
	Q_OBJECT

public:
	TabButton(const char* name, Icon::Ui icon, QWidget* parent = nullptr,
		Icon::Ui flag = Icon::Ui{})
		: Widget(name, parent),
		m_icon(icon),
		m_flag(flag)
	{
		updateIcon();
	}

	bool flagged() const { return m_flagged; }

	void setFlagged(bool flagged)
	{
		m_flagged = flagged;
		updateIcon();
	}

protected:
	virtual void changeEvent(QEvent* event) override
	{
		QToolButton::changeEvent(event);
		if (event->type() == QEvent::StyleChange)
			updateIcon();
	}

	virtual void enterEvent(QEnterEvent* event) override
	{
		QToolButton::enterEvent(event);
		m_hoveredOver = true;
		updateIcon();
	}

	virtual void leaveEvent(QEvent* event) override
	{
		QToolButton::leaveEvent(event);
		m_hoveredOver = false;
		updateIcon();
	}

private:
	Icon::Ui m_icon;
	Icon::Ui m_flag;
	double m_iconScale;
	double m_flagScale;
	bool m_flagged = false;
	bool m_hoveredOver = false;

	void updateIcon()
	{
		(m_flagged && !m_hoveredOver && m_flag != Icon::Ui{})
			? Icon::iconifyButton(m_flag, this)
			: Icon::iconifyButton(m_icon, this);
	}
};
