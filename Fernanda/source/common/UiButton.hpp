#pragma once

#include <QChar>
#include <QDebug>
#include <QEvent>
#include <QEnterEvent>
#include <QFontDatabase>
#include <QString>
#include <QStyle>
#include <QToolButton>

#include <map>

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

	UiButton(const QString& label, QWidget* parent = nullptr, const QString& flag = QString())
		: QToolButton(parent), m_label(label), m_flag(flag)
	{
		updateIcon();
	}

	UiButton(Ui icon, QWidget* parent = nullptr, Ui flag = Ui{})
		: UiButton(iconLabel(icon), parent, iconLabel(flag))
	{
		setFont(uiFont());
	}

	virtual ~UiButton() noexcept
	{
		qDebug() << __FUNCTION__;
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
	const QString m_label;
	const QString m_flag;
	bool m_flagged = false;
	bool m_hoveredOver = false;

	const std::map<Ui, QChar> iconHexMap() const
	{
		return {
		{ Ui::Add, QChar(0xe145) },
		{ Ui::ChevronLeft, QChar(0xe5cb) },
		{ Ui::ChevronDown, QChar(0xe5cf) },
		{ Ui::ChevronRight, QChar(0xe5cc) },
		{ Ui::ChevronUp, QChar(0xe5ce) },
		{ Ui::Close, QChar(0xe5cd) },
		{ Ui::Ellipse, QChar(0xe061) },
		{ Ui::Refresh, QChar(0xe5d5) }
		};
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
