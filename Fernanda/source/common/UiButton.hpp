#pragma once

#include "Widget.hpp"

#include <QChar>
#include <QEnterEvent>
#include <QFontDatabase>
#include <QToolButton>

class UiButton : public Widget<QToolButton>
{
	Q_OBJECT

public:
	enum class Ui {
		None,
		Add,
		ChevronLeft,
		ChevronDown,
		ChevronRight,
		ChevronUp,
		Close,
		Ellipse,
		Refresh
	};

	UiButton(const char* name, const QChar& icon, QWidget* parent = nullptr,
		const QChar& flag = QChar())
		: Widget(name, parent), m_icon(icon), m_flag(flag)
	{
		updateIcon();
	}

	UiButton(const char* name, Ui icon, QWidget* parent = nullptr,
		Ui flag = Ui{})
		: UiButton(name, fontIcon(icon), parent, fontIcon(flag))
	{
		setFont(uiFont());
	}

	bool flagged() const
	{
		return m_flagged;
	}

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
	const std::map<Ui, QChar> uiFontChars = {
		{ Ui::Add, QChar(0xe145) },
		{ Ui::ChevronLeft, QChar(0xe5cb) },
		{ Ui::ChevronDown, QChar(0xe5cf) },
		{ Ui::ChevronRight, QChar(0xe5cc) },
		{ Ui::ChevronUp, QChar(0xe5ce) },
		{ Ui::Close, QChar(0xe5cd) },
		{ Ui::Ellipse, QChar(0xe061) },
		{ Ui::Refresh, QChar(0xe5d5) }
	};
	bool m_isUiFont = false;
	const QChar m_icon;
	const QChar m_flag;
	//double m_iconScale;
	//double m_flagScale;
	bool m_flagged = false;
	bool m_hoveredOver = false;

	const QChar fontIcon(Ui icon)
	{
		QChar font_icon;
		auto it = uiFontChars.find(icon);
		if (it != uiFontChars.end())
			font_icon = it->second;
		return font_icon;
	}

	const QFont uiFont()
	{
		static const auto id = QFontDatabase::addApplicationFont(":/ui/MaterialIcons-Regular.ttf");
		return QFont(QFontDatabase::applicationFontFamilies(id).at(0));
	}

	void updateIcon()
	{
		(m_flagged && !m_hoveredOver && !m_flag.isNull())
			? setText(m_flag)
			: setText(m_icon);
	}
};
