#pragma once

#include "../common/Event.hpp"
#include "../common/Widget.hpp"

#include <QPlainTextEdit>
#include <QPropertyAnimation>
#include <QPushButton>
#include <QScrollBar>

class ScrollButton : public Widget<QPushButton>
{
	Q_OBJECT

public:
	enum class Type {
		Down,
		Next,
		Previous,
		Up
	};

	inline ScrollButton(QPlainTextEdit* editor, Type type, QWidget* parent = nullptr)
		: Widget(parent), m_type(type), m_scrollBar(editor->verticalScrollBar())
	{
		// lines on previous & next
		// needs same color as stylesheet, though

		if (type == Type::Up || type == Type::Previous) {
			setText("\U000025B2");
			editor->addScrollBarWidget(this, Qt::AlignTop);
		}
		else {
			setText("\U000025BC");
			editor->addScrollBarWidget(this, Qt::AlignBottom);
		}
		if (type == Type::Up || type == Type::Down) {
			setAutoRepeat(true);
			setAutoRepeatDelay(500);
		}
		setMinimumHeight(30);
		connections();
	}

private:
	Type m_type;
	QScrollBar* m_scrollBar;

	inline void connections()
	{
		connect(this, &ScrollButton::clicked, this, &ScrollButton::scroll);
		if (m_type == Type::Up || m_type == Type::Down) {
			connectMultiple(m_scrollBar, this,
				&ScrollButton::toggleEnabled, &QScrollBar::rangeChanged, &QScrollBar::valueChanged);
		}
	}

	inline void skip()
	{
		if (m_type == Type::Previous)
			m_scrollBar->setValue(m_scrollBar->minimum());
		if (m_type == Type::Next)
			m_scrollBar->setValue(m_scrollBar->maximum());
	}

	inline bool isMaximumScroll() const { return (m_scrollBar->sliderPosition() == m_scrollBar->maximum()); }
	inline bool isMinimumScroll() const { return (m_scrollBar->sliderPosition() == m_scrollBar->minimum()); }

private slots:
	inline void toggleEnabled()
	{
		if (m_type == Type::Up)
			isMinimumScroll() ? setEnabled(false) : setEnabled(true);
		if (m_type == Type::Down)
			isMaximumScroll() ? setEnabled(false) : setEnabled(true);
	}

	inline void scroll()
	{
		if (m_type == Type::Previous || m_type == Type::Next) {
			skip();
			return;
		}
		auto scroll_value = m_scrollBar->value();
		auto steps = m_scrollBar->singleStep() * 10;
		auto sliding = new QPropertyAnimation(m_scrollBar, "sliderPosition");
		sliding->setDuration(200);
		sliding->setEasingCurve(QEasingCurve::OutQuad);
		sliding->setStartValue(scroll_value);
		(m_type == Type::Up)
			? sliding->setEndValue(scroll_value + steps)
			: sliding->setEndValue(scroll_value - steps);
		sliding->start(QAbstractAnimation::DeleteWhenStopped);
	}
};
