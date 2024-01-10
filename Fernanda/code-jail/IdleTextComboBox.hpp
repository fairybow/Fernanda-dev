#pragma once

#include <QComboBox>
#include <QDebug>
#include <QString>
#include <QStyleOptionComboBox>
#include <QStylePainter>

class IdleTextComboBox : public QComboBox
{
	Q_OBJECT

public:
	IdleTextComboBox(const QString& idleText, QWidget* parent = nullptr)
		: QComboBox(parent), m_idleText(idleText)
	{}

	IdleTextComboBox(QWidget* parent = nullptr)
		: IdleTextComboBox(QString(), parent)
	{}

	~IdleTextComboBox() { qDebug() << __FUNCTION__; }

	QString idleText() const
	{
		return m_idleText;
	}

	void setIdleText(const QString& idleText)
	{
		m_idleText = idleText;
	}

protected:
	void paintEvent(QPaintEvent* event) override
	{
		if (m_idleText.isEmpty()) {
			QComboBox::paintEvent(event);

			return;
		}

		QStylePainter painter(this);
		QStyleOptionComboBox option;

		initStyleOption(&option);
		option.currentText = m_idleText;

		painter.drawComplexControl(QStyle::CC_ComboBox, option);
		painter.drawControl(QStyle::CE_ComboBoxLabel, option);
	}

private:
	QString m_idleText;
};
