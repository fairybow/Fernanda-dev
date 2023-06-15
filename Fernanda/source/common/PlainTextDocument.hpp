#pragma once

#include <QString>
#include <QTextDocument>

class PlainTextDocument : public QTextDocument
{
public:
	struct CursorSpan {
		int cursor = 0;
		int anchor = 0;
	};

	PlainTextDocument(const QString& text, const QString& originalText, const QString& title = QString(), QObject* parent = nullptr)
		: QTextDocument(text, parent),
		m_originalText(originalText),
		m_title(title) {}

	// title field

	// save current document, retrieve next document and send to editor and tabbar to get needed info

	QString originalText() const { return m_originalText; }
	bool edited() const { return toPlainText() == m_originalText; }
	CursorSpan cursorSpan() const { return m_cursorSpan; }

	void setCursorSpan(int cursor, int anchor) { m_cursorSpan = CursorSpan(cursor, anchor); }

private:
	const QString m_originalText;
	QString m_title;
	CursorSpan m_cursorSpan{};
};
