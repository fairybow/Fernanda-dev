#pragma once

#include "../common/Io.hpp"
#include "../common/Path.hpp"
#include "DocumentCache.hpp"
#include "TextDocument.hpp"

#include <QString>
#include <QTimer>
#include <QUuid>
#include <QVector>

#include <algorithm>
#include <filesystem>
#include <map>

// file system watcher

class Document : public QObject
{
	using StdFsPath = std::filesystem::path;

	Q_OBJECT

public:
	Document(StdFsPath tempFolder, StdFsPath backupFolder, QWidget* parent = nullptr);

	const QString open(StdFsPath path);
	const QString open(QUuid id);
	void setText(const QString& text);
	void writeEmptyFile(StdFsPath path);
	QUuid createEmpty();

	QUuid currentId() const { return m_currentId; }

signals:
	void askSetText();
	void startAutoCacheTimer();

private:
	DocumentCache m_cache;
	const StdFsPath m_tempFolder;
	const StdFsPath m_backupFolder;
	QUuid m_currentId;
	std::map<StdFsPath, QUuid> m_extantPathsToIds;
	QVector<QUuid> m_lifetimeIdRegistry;
	QTimer* m_autoCacheText = new QTimer(this);

	void setUpAutoCache();
	const QString read(StdFsPath path = StdFsPath());
	QUuid createId(StdFsPath path = StdFsPath());
	QUuid idByPath(StdFsPath path);
	TextDocument* textDocument(QUuid id, StdFsPath path = StdFsPath());
	void tempSave(QUuid id, const QString& text);
	StdFsPath tempPath(QUuid id);
	TextDocument* create(QUuid id, StdFsPath path = StdFsPath());
	bool recoverIfEvicted(QUuid id, QString& initialText, QString& originalText);
};
