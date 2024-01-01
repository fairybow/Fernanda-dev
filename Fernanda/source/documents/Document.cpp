#include "Document.h"

#include <QPlainTextDocumentLayout>

Document::Document(QObject* parent)
	: QTextDocument(parent)
{
	setup();
}

bool Document::isUntitled() const
{
	return (m_title.isNull() || m_title.isEmpty());
}

bool Document::hasTitle() const
{
	return !isUntitled();
}

QString Document::text() const
{
	return toPlainText();
}

void Document::setText(const QString& text)
{
	setPlainText(text);
}

QString Document::title() const
{
	return m_title;
}

void Document::setTitle(const QString& text)
{
	m_title = text;
}

Path Document::path() const
{
	return m_path;
}

void Document::setPath(const Path& path)
{
	m_path = path;
}

void Document::setup()
{
	auto layout = new QPlainTextDocumentLayout(this);
	setDocumentLayout(layout);
}
