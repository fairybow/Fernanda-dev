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

	PlainTextDocument(const QString& text, const QString& originalText, QObject* parent = nullptr)
		: QTextDocument(text, parent),
		m_originalText(originalText) {}

	// title field
	QString originalText() const { return m_originalText; }
	bool edited() const { return toPlainText() == m_originalText; }
	CursorSpan cursorSpan() const { return m_cursorSpan; }

	void setCursorSpan(int cursor, int anchor) { m_cursorSpan = CursorSpan(cursor, anchor); }

private:
	const QString m_originalText;
	CursorSpan m_cursorSpan{};
};
