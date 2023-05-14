#pragma once

#include "../common/Io.hpp"
#include "../common/Path.hpp"
#include "DocumentCache.hpp"

#include <QString>
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

	void saveCurrent(const QString& text)
	{
		if (m_currentPath.empty()) return;
		auto id = findId(m_currentPath);
		auto document = textDocument(id, m_currentPath);
		document->setPlainText(text);
		tempSave(id, text);
	}

	void openNew()
	{

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
	//std::map<int, QUuid> m_unsavedIds; // not best way. how to keep track of them outside this class (with tab bar--data for if new?)
	// perhaps if new, return the QUuid and store that as the tab data
	std::map<StdFsPath, QUuid> m_pathsToIds;

	QUuid findId(StdFsPath path)
	{
		QUuid id;
		auto it = m_pathsToIds.find(path);
		if (it != m_pathsToIds.end())
			id = it->second;
		else {
			id = QUuid::createUuid(); // QUuid::WithoutBraces
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

	void tempSave(QUuid id, const QString& text)
	{
		Io::writeFile(tempPath(id), text);
	}

	StdFsPath tempPath(QUuid id)
	{
		return m_tempFolder / Path::toStdFs(id.toString() + ".txt~");
	}
};
