#pragma once

#include <QDebug>
#include <QString>
#include <QVariant>

#include <ostream>
#include <string>

class HtmlString
{
public:
	HtmlString() : m_string(QString()) {}
	HtmlString(const char* cString) : m_string(cString) {}
	HtmlString(const std::string& string) : m_string(QString::fromStdString(string)) {}
	HtmlString(const QString& qString) : m_string(qString) {}
	HtmlString(const QVariant& qVariant) : m_string(qVariant.toString()) {}

	operator QString() const
	{
		return m_string;
	}

	operator QVariant() const
	{
		return toQVariant();
	}

	explicit operator bool() const
	{
		return !m_string.isEmpty();
	}

	bool operator==(const HtmlString& other) const
	{
		return m_string == other.m_string;
	}

	bool operator!=(const HtmlString& other) const
	{
		return !(*this == other);
	}

	HtmlString& operator=(const HtmlString& other)
	{
		if (this != &other)
			m_string = other.m_string;

		return *this;
	}

	HtmlString operator%(const HtmlString& rhs) const
	{
		return HtmlString(m_string + P + rhs.m_string);
	}

	HtmlString& operator%=(const HtmlString& rhs)
	{
		*this = *this % rhs;

		return *this;
	}

	HtmlString operator/(const HtmlString& rhs) const
	{
		return HtmlString(m_string + BR + rhs.m_string);
	}

	QString toQString() const
	{
		return m_string;
	}

	QVariant toQVariant() const
	{
		return QVariant::fromValue(m_string);
	}

	std::string toString() const
	{
		return m_string.toStdString();
	}

private:
	static constexpr char BR[] = "<br>";
	static constexpr char BR$[] = "</br>";
	static constexpr char FORMAT_BOLD[] = "<b>%1</b>";
	static constexpr char FORMAT_HEADING[] = "<h%1>%2</h%1>";
	static constexpr char FORMAT_LINK[] = "<a href='%1'>%2</a>";
	static constexpr char P[] = "<p>";
	static constexpr char P$[] = "</p>";

	QString m_string;
};
