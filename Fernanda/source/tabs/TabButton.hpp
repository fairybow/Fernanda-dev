#pragma once

#include "../common/Svg.hpp"

#include <QEnterEvent>
#include <QTabBar>
#include <QToolButton>
#include <QUuid>

class TabButton : public QToolButton
{
	Q_OBJECT

public:
	TabButton(QUuid id, QWidget* parent = nullptr)
		: QToolButton(parent), m_id(id)
	{
		updateIcon();
		connect(this, &QToolButton::clicked, this, [&] {
			emit askClose(m_id);
			});
	}

	bool edited() const { return m_edited; }

	void setEdited(bool edited)
	{
		m_edited = edited;
		updateIcon();
	}

signals:
	void askClose(QUuid id);

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
	QUuid m_id;
	bool m_edited = false;
	bool m_hoveredOver = false;

	void updateIcon()
	{
		auto edited_flag = Svg::ui(Svg::Ui::Ellipse, Qt::red, 0.5);
		auto close = Svg::ui(Svg::Ui::Close, Qt::red);
		setIcon((m_edited && !m_hoveredOver) ? edited_flag : close);
	}
};
