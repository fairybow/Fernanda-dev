#include "Document.h"

Document::Document(StdFsPath tempFolder, StdFsPath backupFolder, QWidget* parent)
	: m_tempFolder(tempFolder), m_backupFolder(backupFolder), m_cache(100)
{
	setUpAutoCache();
}

const QString Document::open(StdFsPath path)
{
	emit askSetText();
	m_currentId = idByPath(path);
	return read(path);
}

const QString Document::open(QUuid id)
{
	emit askSetText();
	m_currentId = id;
	return read();
}

void Document::setText(const QString& text)
{
	if (m_currentId.isNull()) return;
	auto document = textDocument(m_currentId);
	document->setPlainText(text);
	//tempSave(id, text); // when do we really need to temp save?
}

void Document::writeEmptyFile(StdFsPath path)
{
	Io::writeFile(path);
}

QUuid Document::create()
{
	auto id = QUuid::createUuid();
	textDocument(id);
	return id;
}

void Document::setUpAutoCache()
{
	connect(this, &Document::startAutoCacheTimer, this, [&] {
		m_autoCacheText->start(25000);
		});
	connect(m_autoCacheText, &QTimer::timeout, this, [&] {
		emit askSetText();
		});
}

const QString Document::read(StdFsPath path)
{
	auto document = textDocument(m_currentId, path);
	emit startAutoCacheTimer();
	return document->toPlainText();
}

QUuid Document::idByPath(StdFsPath path)
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

QTextDocument* Document::textDocument(QUuid id, StdFsPath path)
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

void Document::tempSave(QUuid id, const QString& text)
{
	Io::writeFile(tempPath(id), text);
}

Document::StdFsPath Document::tempPath(QUuid id)
{
	return m_tempFolder / Path::toStdFs(id.toString() + ".txt~");
}
