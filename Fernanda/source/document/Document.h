#pragma once

#include "../common/Io.hpp"
#include "../common/Path.hpp"
#include "../common/StringTools.hpp"
#include "DocumentCache.hpp"
#include "TextDocument.hpp"

#include <QFileDialog>
#include <QMainWindow>
#include <QString>
#include <QTimer>
#include <QUuid>
#include <QVector>

#include <algorithm>
#include <filesystem>
#include <map>

namespace StdFs = std::filesystem;

class Document : public QObject
{
	Q_OBJECT

public:
	using StdFsPath = StdFs::path;

	struct Folders {
		StdFsPath user;
		StdFsPath temp;
		StdFsPath backup;
	};

	Document(const Folders& folders, QMainWindow* mainWindow, QWidget* parent = nullptr, int cacheMaxCost = 100);

	StdFsPath newFileDialog();
	StdFsPath openFileDialog();
	const QString setCurrent(const StdFsPath& path, bool isNew = false);
	const QString setCurrent(QUuid id);
	void setText(const QString& text);
	QUuid createEmpty();
	void affirmEditedState(const QString& text);
	void startEditCheckTimer();
	bool isSaveable();
	bool save();
	void close(QUuid id);

	QUuid currentId() const { return m_currentId; }
	bool isEdited(QUuid id) { return textDocument(id)->edited(); }

	void setEditCheckDelay(int textLength);

	void devClass()
	{
		qDebug() << __FUNCTION__;
		qDebug() << "Current edit-check delay interval:" << m_editCheckDelay->interval();
		if (!m_extantPathsToIds.empty()) {
			qDebug() << "Extant paths to IDs map:";
			for (auto& [path, id] : m_extantPathsToIds)
				qDebug() << "Path:" << Path::toQString(path) << "\n" << id;
		}
	}

	void devCurrentInfo()
	{
		qDebug() << __FUNCTION__;
		qDebug() << "Current ID:" << m_currentId;
		auto document = m_cache.document(m_currentId);
		qDebug() << "Is edited?:" << document->edited();
		qDebug() << "Original text:" << document->originalText();
	}

signals:
	void askSetText();
	void startAutoCacheTimer();
	void editedStateChanged(QUuid id, bool edited);
	void askEditCheck();
	void pathIdAssociated(const StdFsPath& path, QUuid id);

private:
	static constexpr char DIALOG_FILE_TYPE[] = "Plain text file (*.txt)";

	QMainWindow* m_mainWindow;
	DocumentCache m_cache;
	const StdFsPath m_userFolder;
	const StdFsPath m_tempFolder;
	const StdFsPath m_backupFolder;
	QUuid m_currentId;
	std::map<StdFsPath, QUuid> m_extantPathsToIds;
	QVector<QUuid> m_lifetimeIdRegistry;
	QTimer* m_autoSaveText = new QTimer(this);
	QTimer* m_editCheckDelay = new QTimer(this);

	void setUpAutoCache();
	void writeEmptyFile(const StdFsPath& path);
	const QString read(StdFsPath path = StdFsPath());
	QUuid createId(StdFsPath path = StdFsPath());
	QUuid idByPath(const StdFsPath& path);
	TextDocument* textDocument(QUuid id, StdFsPath path = StdFsPath());

	void tempSave(QUuid id, const QString& text);
	StdFsPath tempPath(QUuid id);
	void backup(QUuid id);
	StdFsPath backupPath(const StdFsPath& path);
	bool overwrite(QUuid id);

	TextDocument* create(QUuid id, StdFsPath path = StdFsPath());
	bool wasEvicted(QUuid id);
	void recover(QUuid id, QString& initialText, QString& originalText);
	StdFsPath extantPath(QUuid id);
};
