#include "Document.h"

Document::Document(StdFsPath tempFolder, StdFsPath backupFolder, QWidget* parent, int cacheMaxCost)
	: m_tempFolder(tempFolder), m_backupFolder(backupFolder), m_cache(cacheMaxCost)
{
	setUpAutoCache();
	m_editCheckDelay->setSingleShot(true);
	connect(m_editCheckDelay, &QTimer::timeout, this, [&] {
		emit askEditCheck();
		});
}

const QString Document::setCurrent(StdFsPath path)
{
	emit askSetText();
	m_currentId = idByPath(path);
	return read(path);
}

const QString Document::setCurrent(QUuid id)
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

	tempSave(m_currentId, text);
}

void Document::writeEmptyFile(StdFsPath path)
{
	Io::writeFile(path);
}

QUuid Document::createEmpty()
{
	auto id = createId();
	textDocument(id);
	return id;
}

void Document::affirmEditedState(const QString& text)
{
	if (m_currentId.isNull()) return;
	auto document = textDocument(m_currentId);
	auto new_state = (text != document->originalText());
	if (new_state == document->edited()) return;
	document->setEdited(new_state);
	emit editedStateChanged(m_currentId, new_state);
}

void Document::startEditCheckTimer()
{
	m_editCheckDelay->start();
}

void Document::setEditCheckDelay(int textLength)
{
	(textLength < 10000)
		? m_editCheckDelay->setInterval(0)
		: m_editCheckDelay->setInterval((textLength / 10000) * 1000);
}

void Document::setUpAutoCache()
{
	connect(this, &Document::startAutoCacheTimer, this, [&] {
		m_autoSaveText->start(25000);
		});
	connect(m_autoSaveText, &QTimer::timeout, this, [&] {
		emit askSetText();
		});
}

const QString Document::read(StdFsPath path)
{
	auto document = textDocument(m_currentId, path);
	emit startAutoCacheTimer();
	return document->toPlainText();
}

QUuid Document::createId(StdFsPath path)
{
	auto id = QUuid::createUuid();
	if (!path.empty())
		m_extantPathsToIds[path] = id;
	m_lifetimeIdRegistry << id;
	return id;
}

QUuid Document::idByPath(StdFsPath path)
{
	QUuid id;
	auto it = m_extantPathsToIds.find(path);
	(it != m_extantPathsToIds.end())
		? id = it->second
		: id = createId(path);
	return id;
}

TextDocument* Document::textDocument(QUuid id, StdFsPath path)
{
	auto document = m_cache.document(id);
	if (!document)
		document = create(id, path);
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

TextDocument* Document::create(QUuid id, StdFsPath path)
{
	QString initial_text;
	QString original_text;

	if (wasEvicted(id))
		recover(id, initial_text, original_text);
	else if (!path.empty())
		Io::toStrings(path, initial_text, original_text);

	auto document = new TextDocument(initial_text, original_text);
	m_cache.insertDocument(id, document);
	return document;
}

bool Document::wasEvicted(QUuid id)
{
	if (!m_lifetimeIdRegistry.contains(id)) return false;
	return StdFs::exists(tempPath(id));
}

void Document::recover(QUuid id, QString& initialText, QString& originalText)
{
	auto temp_path = tempPath(id);
	if (StdFs::exists(temp_path))
		initialText = Io::readFile(temp_path);

	auto it = std::find_if(
		m_extantPathsToIds.begin(), m_extantPathsToIds.end(),
		[&id](const auto& pair) { return pair.second == id; });

	if (it != m_extantPathsToIds.end()) {
		auto& extant_path = it->first;
		if (StdFs::exists(extant_path))
			originalText = Io::readFile(extant_path);
		// handle deleted original
		// file system watcher
	}
}
