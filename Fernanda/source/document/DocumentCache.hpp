#pragma once

#include <QCache>
#include <QTextDocument>

class DocumentCache : public QCache<QString, QTextDocument>
{
public:
	DocumentCache(int maxCost)
		: QCache(maxCost) {}

	void insertDocument(const QString& fileName, QTextDocument* document)
	{
		insert(fileName, document);
	}

	QTextDocument* document(const QString& fileName)
	{
		return object(fileName);
	}
};
