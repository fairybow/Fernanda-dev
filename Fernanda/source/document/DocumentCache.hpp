#pragma once

#include "TextDocument.hpp"

#include <QCache>
#include <QUuid>

class DocumentCache : public QCache<QUuid, TextDocument>
{
public:
	DocumentCache(int maxCost = 100)
		: QCache(maxCost) {}

	void insertDocument(const QUuid& fileId, TextDocument* document)
	{
		insert(fileId, document);
	}

	TextDocument* document(const QUuid& fileId)
	{
		return object(fileId);
	}
};
