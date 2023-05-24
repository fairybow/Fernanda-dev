#pragma once

#include <QAbstractButton>
#include <QChar>
#include <QFontDatabase>

#include <map>

namespace Icon
{
	enum class Ui {
		None,
		Add,
		ChevronLeft,
		ChevronDown,
		ChevronRight,
		ChevronUp,
		Close,
		Ellipse
	};

	namespace
	{
		const std::map<Ui, QChar> uiChars = {
		{ Ui::Add, QChar(0xe145) },
		{ Ui::ChevronLeft, QChar(0xe5cb) },
		{ Ui::ChevronDown, QChar(0xe5cf) },
		{ Ui::ChevronRight, QChar(0xe5cc) },
		{ Ui::ChevronUp, QChar(0xe5ce) },
		{ Ui::Close, QChar(0xe5cd) },
		{ Ui::Ellipse, QChar(0xe061) }
		};
	}

	inline const QFont uiFont()
	{
		static const auto id = QFontDatabase::addApplicationFont(":/ui/MaterialIcons-Regular.ttf");
		return QFont(QFontDatabase::applicationFontFamilies(id).at(0));
	}

	inline void iconifyButton(Ui icon, QAbstractButton* button)
	{
		auto it = uiChars.find(icon);
		if (it == uiChars.end()) return;
		button->setText(it->second);
		button->setFont(uiFont());
	}
}
