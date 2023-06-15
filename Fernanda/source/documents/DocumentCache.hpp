#pragma once

#include "../common/PlainTextDocument.hpp"

#include <QCache>
#include <QUuid>

class DocumentCache : public QCache<QUuid, PlainTextDocument>
{
public:
	DocumentCache(int maxCost = 100)
		: QCache(maxCost) {}

	void insertDocument(const QUuid& id, PlainTextDocument* document)
	{
		insert(id, document);
	}

	PlainTextDocument* document(const QUuid& id)
	{
		return object(id);
	}
};
