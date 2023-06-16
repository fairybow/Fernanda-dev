#pragma once

#include <QString>
#include <QTextDocument>
#include <QVariant>

class Document /*DocumentRecord more accurate?*/ : public QTextDocument
{
public:
	struct CursorSpan {
		int cursor = 0;
		int anchor = 0;
	};

	Document(
		const QString& text,
		const QString& originalText,
		const QString& title = QString(),
		QObject* parent = nullptr)
		: QTextDocument(text, parent),
		m_originalText(originalText),
		m_title(title) {}

	// save current document, retrieve next document and send to editor and tabbar to get needed info

	CursorSpan cursorSpan() const { return m_cursorSpan; }
	QVariant data() const { return m_data; }
	bool isEdited() const { return toPlainText() == m_originalText; }
	bool isUntitled() const { return m_title == QString(); }
	QString originalText() const { return m_originalText; }
	QString title() const { return m_title; }

	void setCursorSpan(int cursor, int anchor) { m_cursorSpan = CursorSpan(cursor, anchor); }
	void setData(const QVariant& data) { m_data = data; }
	void setTitle(const QString& title) { m_title = title; }

private:
	CursorSpan m_cursorSpan{};
	QVariant m_data;
	const QString m_originalText;
	QString m_title;
};
