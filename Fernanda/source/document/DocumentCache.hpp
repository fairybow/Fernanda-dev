#pragma once

#include <QCache>
#include <QTextDocument>
#include <QUuid>

class DocumentCache : public QCache<QUuid, QTextDocument>
{
public:
	DocumentCache(int maxCost)
		: QCache(maxCost) {}

	void insertDocument(const QUuid& fileId, QTextDocument* document)
	{
		insert(fileId, document);
	}

	QTextDocument* document(const QUuid& fileId)
	{
		return object(fileId);
	}
};
