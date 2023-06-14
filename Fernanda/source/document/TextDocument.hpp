#pragma once

#include <QString>
#include <QTextDocument>

class TextDocument : public QTextDocument
{
public:
	struct CursorSpan {
		int cursor = 0;
		int anchor = 0;
	};

	TextDocument(const QString& text, const QString& originalText, QObject* parent = nullptr)
		: QTextDocument(text, parent), m_originalText(originalText) {}

	QString originalText() const { return m_originalText; }
	bool edited() const { return m_edited; }
	CursorSpan cursorSpan() const { return m_cursorSpan; }

	void setEdited(bool edited) { m_edited = edited; }
	void setCursorSpan(int cursor, int anchor) { m_cursorSpan = CursorSpan(cursor, anchor); }

private:
	const QString m_originalText;
	bool m_edited = false;
	CursorSpan m_cursorSpan{};
};
