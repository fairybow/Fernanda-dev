#pragma once

#include <QPainter>
#include <QToolButton>

class TabButton : public QToolButton
{
public:
	TabButton(QWidget* parent = nullptr)
		: QToolButton(parent) {}

	bool edited() const { return m_edited; }

	void setEdited(bool edited) { m_edited = edited; }

protected:
	void paintEvent(QPaintEvent* event) override
	{
		// if edited
		QPainter painter(this);
		painter.setBrush(Qt::red);
		painter.drawEllipse(rect());
		// remove flag on hover
	}

private:
	bool m_edited = false;
};
