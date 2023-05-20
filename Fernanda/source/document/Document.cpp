#include "Document.h"

Document::Document(StdFsPath tempFolder, StdFsPath backupFolder, QWidget* parent)
	: m_tempFolder(tempFolder), m_backupFolder(backupFolder), m_cache(3) /* <-- test */
{
	setUpAutoCache();
}

const QString Document::serve(StdFsPath path)
{
	emit askSetText();
	m_currentId = idByPath(path);
	return read(path);
}

const QString Document::serve(QUuid id)
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

bool Document::editedState(const QString& text)
{
	if (m_currentId.isNull()) return false;
	auto document = textDocument(m_currentId);
	if (text != document->originalText())
		return true;
	return false;
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
	QString original_text;
	QString initial_text;

	auto recover = recoverIfEvicted(id, initial_text, original_text); // <-- can't run this because it will appear in the registry

	//

	// change to bool isEvicted() to check.

	//
	if (!recover && !path.empty())
		Io::toStrings(path, initial_text, original_text);

	qDebug() << recover;
	qDebug() << path;
	qDebug() << id;
	qDebug() << initial_text;
	qDebug() << original_text;

	auto document = new TextDocument(initial_text, original_text);
	m_cache.insertDocument(id, document);
	return document;
}

bool Document::recoverIfEvicted(QUuid id, QString& initialText, QString& originalText)
{
	if (!m_lifetimeIdRegistry.contains(id)) return false;

	auto temp_path = tempPath(id);
	if (std::filesystem::exists(temp_path))
		initialText = Io::readFile(temp_path);

	auto it = std::find_if(
		m_extantPathsToIds.begin(), m_extantPathsToIds.end(),
		[&id](const auto& pair) { return pair.second == id; });
	if (it != m_extantPathsToIds.end()) {
		auto& extant_path = it->first;
		if (std::filesystem::exists(extant_path))
			originalText = Io::readFile(extant_path);
	}
	return true;
}
