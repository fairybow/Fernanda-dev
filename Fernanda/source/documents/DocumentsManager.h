#pragma once

#include "../common/Io.hpp"
#include "../common/Path.hpp"
#include "../common/Document.hpp"
#include "DocumentsCache.h"
#include "IdBank.hpp"

#include <QFileDialog>
#include <QString>
#include <QUuid>

#include <filesystem>

namespace StdFs = std::filesystem;

class DocumentsManager : public QObject
{
	Q_OBJECT

public:
	using StdFsPath = StdFs::path;

	enum class PathType {
		Extant,
		New
	};

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

	StdFsPath newFileDialog(QWidget* parent = nullptr, const QString& name = QString());
	StdFsPath openFileDialog(QWidget* parent = nullptr);
	Document* setActive(const QUuid& id);
	QUuid newUnsaved();
	QUuid fromDisk(PathType type, const StdFsPath& path);

	void devClass()
	{
		qDebug() << __FUNCTION__;

		//qDebug() << "Current edit-check delay interval:"
			//<< m_editCheckDelay->interval()
			//<< Qt::endl;
	}

	void devCurrentInfo()
	{
		qDebug() << __FUNCTION__;

		qDebug() << "Active ID:" << m_activeId;
		auto document = active();
		qDebug() << "Is edited?:" << document->isEdited();
		// title
		auto span = document->cursorSpan();
		qDebug() << "Cursor position:" << span.cursor;
		qDebug() << "Anchor position:" << span.anchor;
		qDebug() << "Original text:" << document->originalText()
			<< Qt::endl;
	}

	void devIdBank() const
	{
		qDebug() << __FUNCTION__ << Qt::endl;

		if (!s_idBank.isEmpty()) {
			qDebug() << "Bank:";
			for (auto& id : s_idBank.bank())
				qDebug() << id << Qt::endl;
		}
		if (s_idBank.hasTrash()) {
			qDebug() << "Trash:";
			for (auto& id : s_idBank.trash())
				qDebug() << id << Qt::endl;
		}
		if (s_idBank.hasPaths()) {
			qDebug() << "Extant paths to IDs map:";
			for (auto& [path, id] : s_idBank.paths())
				qDebug() << "Path:" << Path::toQString(path)
				<< "\n" << id << Qt::endl;
		}
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

	Document* retrieve(const QUuid& id, const StdFsPath& path = StdFsPath());
	Document* newDocument(const QUuid& id, const StdFsPath& path = StdFsPath());
	bool wasEvicted(const QUuid& id);
	StdFsPath tempPath(const QUuid& id);
	void recover(const QUuid& id, QString& initialText, QString& originalText, QString& title);
	bool writeEmptyFile(const StdFsPath& path);
};

using DocsManager = DocumentsManager;
