#pragma once

#include "../common/Io.hpp"
#include "DocumentCache.hpp"

#include <QString>
#include <QUuid>

#include <filesystem>
#include <map>

class Document
{
	using StdFsPath = std::filesystem::path;

public:
	Document(StdFsPath tempFolder, StdFsPath backupFolder, QWidget* parent = nullptr)
		: m_tempFolder(tempFolder), m_backupFolder(backupFolder), m_cache(100) {}

	void saveCurrent(const QString& text)
	{
		if (m_currentPath.empty()) return;
		auto document = textDocument(findId(m_currentPath), m_currentPath);
		document->setPlainText(text);
		// temp save at some point, maybe here, maybe not
		//Io::writeFile(temp_path, text);
	}

	const QString open(StdFsPath path)
	{
		m_currentPath = path;
		auto id = findId(path);
		auto document = textDocument(id, m_currentPath);
		return document->toPlainText();
	}

private:
	DocumentCache m_cache;
	const StdFsPath m_tempFolder;
	const StdFsPath m_backupFolder;
	StdFsPath m_currentPath;
	std::map<StdFsPath, QUuid> m_pathsToIds;

	QUuid findId(StdFsPath path)
	{
		QUuid id;
		auto it = m_pathsToIds.find(path);
		if (it != m_pathsToIds.end())
			id = it->second;
		else {
			id = QUuid::createUuid();
			m_pathsToIds[path] = id;
		}
		return id;
	}

	QTextDocument* textDocument(QUuid id, StdFsPath path)
	{
		auto document = m_cache.document(id);
		if (!document) {
			auto text = Io::readFile(path);
			document = new QTextDocument(text);
			m_cache.insertDocument(id, document);
		}
		return document;
	}
};
