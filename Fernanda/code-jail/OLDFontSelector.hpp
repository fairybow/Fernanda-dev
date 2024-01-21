/*#pragma once

#include "../../common/Layout.hpp"

#include <QComboBox>
#include <QDebug>
#include <QFont>
#include <QFontDatabase>
#include <QList>
#include <QString>
#include <QWidget>

class FontSelector : public QWidget
{
	Q_OBJECT

public:

	int currentIndex() const
	{
		return m_familiesBox->currentIndex();
	}

	void setCurrentIndex(int index)
	{
		m_familiesBox->setCurrentIndex(index);
	}

	QString currentText()
	{
		return m_familiesBox->currentText();
	}

	void setCurrentText(const QString& text)
	{
		m_familiesBox->setCurrentText(text);
	}

private:


};

style.contains("Bold")
			? m_currentFont.setBold(true)
			: m_currentFont.setBold(false);

		style.contains("Italic")
			? m_currentFont.setItalic(true)
			: m_currentFont.setItalic(false);

		style.contains("Thin")
			? m_currentFont.setWeight(QFont::Thin)
			: m_currentFont.setWeight(QFont::Normal);

		style.contains("Extra light")
			? m_currentFont.setWeight(QFont::ExtraLight)
			: m_currentFont.setWeight(QFont::Normal);

		style.contains("Light")
			? m_currentFont.setWeight(QFont::Light)
			: m_currentFont.setWeight(QFont::Normal);

		style.contains("Medium")
			? m_currentFont.setWeight(QFont::Medium)
			: m_currentFont.setWeight(QFont::Normal);

		style.contains("Demi bold")
			? m_currentFont.setWeight(QFont::DemiBold)
			: m_currentFont.setWeight(QFont::Normal);

		style.contains("Bold")
			? m_currentFont.setWeight(QFont::Bold)
			: m_currentFont.setWeight(QFont::Normal);

		style.contains("Extra bold")
			? m_currentFont.setWeight(QFont::ExtraBold)
			: m_currentFont.setWeight(QFont::Normal);

		style.contains("Black")
			? m_currentFont.setWeight(QFont::Black)
			: m_currentFont.setWeight(QFont::Normal);
*/