#pragma once

#include "../common/Io.hpp"
#include "../common/Path.hpp"
#include "DocumentCache.hpp"

#include <QString>
#include <QTextDocument>
#include <QUuid>

#include <filesystem>
#include <map>

// file system watcher

class Document
{
	using StdFsPath = std::filesystem::path;

public:
	Document(StdFsPath tempFolder, StdFsPath backupFolder, QWidget* parent = nullptr)
		: m_tempFolder(tempFolder), m_backupFolder(backupFolder), m_cache(100) {}

	void setText(const QString& text)
	{
		if (m_currentId.isNull()) return;
		auto document = textDocument(m_currentId);
		document->setPlainText(text);
		//tempSave(id, text);
	}

	const QString open(StdFsPath path)
	{
		m_currentId = idByPath(path);
		auto document = textDocument(m_currentId, path);
		return document->toPlainText();
	}

	const QString open(QUuid id)
	{
		m_currentId = id;
		auto document = textDocument(m_currentId);
		return document->toPlainText();
	}

	void writeEmpty(StdFsPath path)
	{
		Io::writeFile(path);
	}

	QUuid create()
	{
		auto id = QUuid::createUuid();
		textDocument(id);
		return id;
	}

	QUuid currentId() const { return m_currentId; }

private:
	DocumentCache m_cache;
	const StdFsPath m_tempFolder;
	const StdFsPath m_backupFolder;
	QUuid m_currentId;
	std::map<StdFsPath, QUuid> m_extantPathsToIds;

	QUuid idByPath(StdFsPath path)
	{
		QUuid id;
		auto it = m_extantPathsToIds.find(path);
		if (it != m_extantPathsToIds.end())
			id = it->second;
		else {
			id = QUuid::createUuid();
			m_extantPathsToIds[path] = id;
		}
		return id;
	}

	QTextDocument* textDocument(QUuid id, StdFsPath path = StdFsPath())
	{
		auto document = m_cache.document(id);
		if (!document) {
			QString text;
			if (!path.empty())
				text = Io::readFile(path);
			document = new QTextDocument(text);
			m_cache.insertDocument(id, document);
		}
		return document;
	}

	void tempSave(QUuid id, const QString& text)
	{
		Io::writeFile(tempPath(id), text);
	}

	StdFsPath tempPath(QUuid id)
	{
		return m_tempFolder / Path::toStdFs(id.toString() + ".txt~");
	}
};
