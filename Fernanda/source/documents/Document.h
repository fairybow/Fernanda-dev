#pragma once

#include "../common/Path.hpp"

#include <QDebug>
#include <QString>
#include <QTextDocument>

class Document : public QTextDocument
{
	Q_OBJECT

public:
	Document(QObject* parent = nullptr);
	~Document() { qDebug() << __FUNCTION__; }

	bool isUntitled() const;
	bool hasTitle() const;

	QString text() const;
	void setText(const QString& text);
	QString title() const;
	void setTitle(const QString& text);
	Path path() const;
	void setPath(const Path& path);

private:
	QString m_title;
	Path m_path;

	void setup();
};
