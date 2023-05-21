#pragma once

#include <QEnterEvent>
#include <QPainter>
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
		updateText();
		connect(this, &QToolButton::clicked, this, [&] {
			emit askClose(m_id);
			});
	}

	bool edited() const { return m_edited; }

	void setEdited(bool edited)
	{
		m_edited = edited;
		updateText();
	}

signals:
	void askClose(QUuid id);

protected:
	virtual void enterEvent(QEnterEvent* event) override
	{
		m_hoveredOver = true;
		updateText();
		QToolButton::enterEvent(event);
	}

	virtual void leaveEvent(QEvent* event) override
	{
		m_hoveredOver = false;
		updateText();
		QToolButton::leaveEvent(event);
	}

private:
	QUuid m_id;
	bool m_edited = false;
	bool m_hoveredOver = false;

	void updateText()
	{
		auto edited_flag = QString::fromUtf8("\u25CF");
		setText((m_edited && !m_hoveredOver) ? edited_flag : "x");
	}
};
