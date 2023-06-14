#pragma once

#include "../common/Io.hpp"
#include "../common/Path.hpp"
#include "../common/StringTools.hpp"
#include "DocumentCache.hpp"
#include "IdBank.hpp"
#include "TextDocument.hpp"

#include <QFileDialog>
#include <QMainWindow>
#include <QString>
#include <QTextBlock>
#include <QTimer>
#include <QUuid>

#include <filesystem>

namespace StdFs = std::filesystem;

class Document : public QObject
{
	Q_OBJECT

public:
	using StdFsPath = StdFs::path;
	using CursorSpan = TextDocument::CursorSpan;

	struct Folders {
		StdFsPath user;
		StdFsPath temp;
		StdFsPath backup;
	};

	Document(const Folders& folders, QMainWindow* mainWindow, QWidget* parent = nullptr, int cacheMaxCost = 100);

	StdFsPath newFileDialog(const QString& name = QString());
	StdFsPath openFileDialog();
	const QString setCurrent(const StdFsPath& path, bool isNew = false);
	const QString setCurrent(const QUuid& id);
	const CursorSpan cursorSpan();
	void setText(const QString& text);
	void setCursorSpan(int cursor, int anchor);
	QUuid createEmpty();
	void affirmEditedState(const QString& text);
	void startEditCheckTimer();
	bool isSaveable();
	bool save();
	void close(const QUuid& id);

	QUuid currentId() const { return m_currentId; }
	bool isEdited(const QUuid& id) { return textDocument(id)->edited(); }

	void setEditCheckDelay(int textLength);

	void devClass()
	{
		qDebug() << __FUNCTION__;

		qDebug() << "Current edit-check delay interval:"
			<< m_editCheckDelay->interval()
			<< Qt::endl;
	}

	void devCurrentInfo()
	{
		qDebug() << __FUNCTION__;

		qDebug() << "Current ID:" << m_currentId;
		auto document = m_cache.document(m_currentId);
		qDebug() << "Is edited?:" << document->edited();
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
	void askSetText();
	void askSetCursorSpan();
	void startAutoCacheTimer();
	void editedStateChanged(const QUuid& id, bool edited);
	void askEditCheck();
	void pathAndIdAssociated(const StdFsPath& path, const QUuid& id);

private:
	static constexpr char DIALOG_FILE_TYPE[] = "Plain text file (*.txt)";

	static IdBank s_idBank;
	QMainWindow* m_mainWindow;
	DocumentCache m_cache;
	const StdFsPath m_userFolder;
	const StdFsPath m_tempFolder;
	const StdFsPath m_backupFolder;
	QUuid m_currentId;
	QTimer* m_autoSaveText = new QTimer(this);
	QTimer* m_editCheckDelay = new QTimer(this);

	void setUpAutoCache();
	void writeEmptyFile(const StdFsPath& path);
	const QString read(StdFsPath path = StdFsPath());
	TextDocument* textDocument(const QUuid& id, StdFsPath path = StdFsPath());

	void tempSave(const QUuid& id, const QString& text);
	StdFsPath tempPath(const QUuid& id);
	void backup(const QUuid& id);
	StdFsPath backupPath(const StdFsPath& path);
	bool overwrite(const QUuid& id);

	TextDocument* create(const QUuid& id, StdFsPath path = StdFsPath());
	bool wasEvicted(const QUuid& id);
	void recover(const QUuid& id, QString& initialText, QString& originalText);
};
