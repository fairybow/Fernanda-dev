#pragma once

#include "Widget.hpp"

#include <QEvent>
#include <QGraphicsOpacityEffect>
#include <QPushButton>

class UiButton : public Widget<QPushButton> // rename UiButton. can leave separate qss for statusbar vs tab vs other
{
	Q_OBJECT

public:
	UiButton(const char* name, const QString& text, QWidget* parent, double defaultOpacity = 0.5)
		: Widget(name, parent), m_opacity(defaultOpacity)
	{
		installEventFilter(this);
		setText(text);
		m_effect->setOpacity(m_opacity);
		setGraphicsEffect(m_effect);
		connect(this, &UiButton::toggled, this, [&](bool checked) {
			m_effect->setEnabled(!m_effect->isEnabled());
			});
	}

protected:
	virtual bool eventFilter(QObject* object, QEvent* event) override
	{
		if (qobject_cast<UiButton*>(object)
			&& (event->type() == QEvent::Enter || event->type() == QEvent::Leave))
			m_effect->setEnabled(!m_effect->isEnabled());
		return QPushButton::eventFilter(object, event);
	}

private:
	QGraphicsOpacityEffect* m_effect = new QGraphicsOpacityEffect(this);
	double m_opacity = 0.0;
};
