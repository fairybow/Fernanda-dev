#pragma once

#include "TextDocument.hpp"

#include <QCache>
#include <QUuid>

class DocumentCache : public QCache<QUuid, TextDocument>
{
public:
	DocumentCache(int maxCost = 100)
		: QCache(maxCost) {}

	void insertDocument(const QUuid& id, TextDocument* document)
	{
		insert(id, document);
	}

	TextDocument* document(const QUuid& id)
	{
		return object(id);
	}
};
