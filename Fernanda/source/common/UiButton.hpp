#pragma once

#include <QChar>
#include <QDebug>
#include <QEvent>
#include <QEnterEvent>
#include <QFontDatabase>
#include <QString>
#include <QStyle>
#include <QToolButton>

#include <unordered_map>

constexpr char UI_FONT_QRC_PATH[] = ":/external/material-icons/MaterialIcons-Regular.ttf";

class UiButton : public QToolButton
{
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

	UiButton(const QString& text, QWidget* parent = nullptr, const QString& flaggedText = QString())
		: QToolButton(parent), m_label(text), m_flag(flaggedText)
	{
		updateIcon();
	}

	UiButton(Ui icon, QWidget* parent = nullptr, Ui flag = Ui{})
		: UiButton(iconLabel(icon), parent, iconLabel(flag))
	{
		setFont(uiFont());
	}

	virtual ~UiButton()
	{
		qDebug() << __FUNCTION__;
	}

	bool hoveredOver() const
	{
		return m_hoveredOver;
	}

	QString label() const
	{
		return m_label;
	}

	void setLabel(const QString& text)
	{
		m_label = text;
	}

	void setLabel(Ui icon)
	{
		m_label = iconLabel(icon);
	}

	QString flag() const
	{
		return m_flag;
	}

	void setFlag(const QString& text)
	{
		m_flag = text;
	}

	void setFlag(Ui icon)
	{
		m_flag = iconLabel(icon);
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
	QString m_label;
	QString m_flag;
	bool m_flagged = false;
	bool m_hoveredOver = false;

	const std::unordered_map<Ui, QChar> iconHexMap() const
	{
		static const std::unordered_map<Ui, QChar> map = {
			{ Ui::Add, QChar(0xe145) },
			{ Ui::ChevronLeft, QChar(0xe5cb) },
			{ Ui::ChevronDown, QChar(0xe5cf) },
			{ Ui::ChevronRight, QChar(0xe5cc) },
			{ Ui::ChevronUp, QChar(0xe5ce) },
			{ Ui::Close, QChar(0xe5cd) },
			{ Ui::Ellipse, QChar(0xe061) },
			{ Ui::Refresh, QChar(0xe5d5) }
		};

		return map;
	}

	const QString iconLabel(Ui icon) const
	{
		auto map = iconHexMap();
		QChar font_icon;

		auto it = map.find(icon);
		if (it != map.end())
			font_icon = it->second;

		return QString(font_icon);
	}

	const QFont uiFont() const
	{
		static const auto id = QFontDatabase::addApplicationFont(UI_FONT_QRC_PATH);

		return QFont(QFontDatabase::applicationFontFamilies(id).at(0));
	}

	bool shouldFlagDisplay() const
	{
		return m_flagged && !m_hoveredOver && !m_flag.isNull();
	}

	void updateIcon()
	{
		shouldFlagDisplay()
			? setText(m_flag)
			: setText(m_label);

		updateFlaggedProperty();
	}

	void updateFlaggedProperty()
	{
		setProperty("flagged", shouldFlagDisplay());

		style()->unpolish(this);
		style()->polish(this);
	}
};
