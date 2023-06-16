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

	DocumentsManager(const Folders& folders, QObject* parent = nullptr, int cacheMaxCost = 100);

	// setting activeId
	// focus on active document; public methods work on activeId

	bool hasActive() const { return !m_activeId.isNull(); }
	Document* active() { return retrieve(m_activeId); }

	Document* setActive(const QUuid& id);
	QUuid newId();

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

	Document* retrieve(const QUuid& id, StdFsPath path = StdFsPath());
	Document* newDocument(const QUuid& id, StdFsPath path = StdFsPath());
	bool wasEvicted(const QUuid& id);
	StdFsPath tempPath(const QUuid& id);
	void recover(const QUuid& id, QString& initialText, QString& originalText, QString& title);
};

using DocsManager = DocumentsManager;
