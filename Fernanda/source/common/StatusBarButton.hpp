#pragma once

#include <QEvent>
#include <QGraphicsOpacityEffect>
#include <QPushButton>

class StatusBarButton : public QPushButton
{
	Q_OBJECT

public:
	inline StatusBarButton(const char* name, const QString& text, QWidget* parent, double defaultOpacity = 0.5)
		: QPushButton(parent), m_opacity(defaultOpacity)
	{
		setObjectName(name);
		installEventFilter(this);
		setText(text);
		m_effect->setOpacity(m_opacity);
		setGraphicsEffect(m_effect);
		connect(this, &StatusBarButton::toggled, this, [&](bool checked) {
			m_effect->setEnabled(!m_effect->isEnabled());
			});
	}

protected:
	inline virtual bool eventFilter(QObject* object, QEvent* event) override
	{
		if (qobject_cast<StatusBarButton*>(object)
			&& (event->type() == QEvent::Enter || event->type() == QEvent::Leave))
			m_effect->setEnabled(!m_effect->isEnabled());
		return QPushButton::eventFilter(object, event);
	}

private:
	QGraphicsOpacityEffect* m_effect = new QGraphicsOpacityEffect(this);
	double m_opacity = 0.0;
};
