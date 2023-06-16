#pragma once

#include "../common/Io.hpp"
#include "../common/Path.hpp"
#include "../common/Document.hpp"
#include "DocumentsCache.h"
#include "IdBank.hpp"

#include <QUuid>

#include <filesystem>

namespace StdFs = std::filesystem;

class DocumentsManager : public QObject
{
	Q_OBJECT

public:
	using StdFsPath = StdFs::path;

	struct Folders {
		StdFsPath user;
		StdFsPath temp;
		StdFsPath backup;
	};

	DocumentsManager(
		const Folders& folders,
		QWidget* parent = nullptr,
		int cacheMaxCost = 100)
		: QObject(parent),
		m_userFolder(folders.user),
		m_tempFolder(folders.temp),
		m_backupFolder(folders.backup)
	{
		DocumentsCache::setMaxCost(cacheMaxCost);
	}

	// setting activeId
	// focus on active document; public methods work on activeId

	Document* activeDocument()
	{
		return document(m_activeId);
	}

signals:
	//

private:
	static constexpr char FILE_TYPE[] = ".txt"; // later save to / pull from document?, for using multiple PT type, .md, .txt, etc.
	static constexpr char DIALOG_FILE_TYPE[] = "Plain text file (*.txt)";

	static IdBank s_idBank;
	const StdFsPath m_userFolder;
	const StdFsPath m_tempFolder;
	const StdFsPath m_backupFolder;

	QUuid m_activeId;

	Document* document(const QUuid& id, StdFsPath path = StdFsPath())
	{
		auto& cache = DocsCache::instance();
		auto document = cache.document(id);
		if (!document)
			document = newDocument(id, path);

		return document;
	}

	Document* newDocument(const QUuid& id, StdFsPath path = StdFsPath())
	{
		QString initial_text;
		QString original_text;
		QString title;

		if (wasEvicted(id))
			recover(id, initial_text, original_text, title);
		else if (!path.empty()) {
			title = Path::qStringName(path);
			Io::toStrings(path, initial_text, original_text);
		}

		auto document = new Document(
			initial_text, original_text, title, id);
		auto& cache = DocsCache::instance();
		cache.add(document);

		return document;
	}

	bool wasEvicted(const QUuid& id)
	{
		if (!s_idBank.contains(id)) return false;
		return StdFs::exists(tempPath(id));
	}

	StdFsPath tempPath(const QUuid& id)
	{
		return m_tempFolder / Path::toStdFs(id.toString() + FILE_TYPE + "~");
	}

	void recover(const QUuid& id, QString& initialText, QString& originalText, QString& title)
	{
		qDebug() << __FUNCTION__;

		auto temp_path = tempPath(id);
		if (StdFs::exists(temp_path))
			initialText = Io::readFile(temp_path);

		auto extant_path = s_idBank.path(id);
		if (Path::isValid(extant_path)) {
			originalText = Io::readFile(extant_path);
			title = Path::qStringName(extant_path);
		}

		// handle deleted original
		// file system watcher
	}
};

using DocsManager = DocumentsManager;
