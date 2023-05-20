#pragma once

#include <QString>
#include <QTextDocument>

class TextDocument : public QTextDocument
{
public:
	TextDocument(const QString& text, const QString& originalText, QObject* parent = nullptr)
		: QTextDocument(text, parent), m_originalText(originalText) {}

	QString originalText() const { return m_originalText; }
	bool edited() const { return m_edited; }

	void setEdited(bool edited) { m_edited = edited; }

private:
	const QString m_originalText;
	bool m_edited = false;
};
