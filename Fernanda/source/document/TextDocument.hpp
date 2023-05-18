#pragma once

#include <QString>
#include <QTextDocument>

class TextDocument : public QTextDocument
{
	Q_OBJECT

public:
	TextDocument(const QString& text, QObject* parent = nullptr)
		: QTextDocument(text, parent), m_originalText(text) {}

	QString originalText() const { return m_originalText; }

private:
	const QString m_originalText;
};
