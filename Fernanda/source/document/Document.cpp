#include "Document.h"

IdBank Document::s_idBank;

Document::Document(const Folders& folders, QMainWindow* mainWindow, QWidget* parent, int cacheMaxCost)
	: QObject(parent),
	m_userFolder(folders.user),
	m_tempFolder(folders.temp),
	m_backupFolder(folders.backup),
	m_mainWindow(mainWindow),
	m_cache(cacheMaxCost)
{
	setUpAutoCache();
	m_editCheckDelay->setSingleShot(true);
	connect(m_editCheckDelay, &QTimer::timeout, this, [&] {
		emit askEditCheck();
		});
}

Document::StdFsPath Document::newFileDialog()
{
	auto path = QFileDialog::getSaveFileName(
		m_mainWindow, tr("Create a new file..."), Path::toQString(
			m_userFolder), tr(DIALOG_FILE_TYPE));
	return Path::toStdFs(path);
}

Document::StdFsPath Document::openFileDialog()
{
	auto path = QFileDialog::getOpenFileName(
		m_mainWindow, tr("Open an existing file..."), Path::toQString(
			m_userFolder), tr(DIALOG_FILE_TYPE));
	return Path::toStdFs(path);
}

const QString Document::setCurrent(const StdFsPath& path, bool isNew)
{
	if (isNew)
		writeEmptyFile(path);

	emit askSetText();
	m_currentId = s_idBank.fromPath(path);
	return read(path);
}

const QString Document::setCurrent(const QUuid& id)
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

QUuid Document::createEmpty()
{
	auto id = s_idBank.create();
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

bool Document::isSaveable()
{
	return (!m_currentId.isNull() && isEdited(m_currentId));
}

bool Document::save()
{
	qDebug() << __FUNCTION__;

	if (m_currentId.isNull()) return false;
	emit askSetText();

	auto extant_path = s_idBank.path(m_currentId);
	if (Path::isValid(extant_path))
		backup(m_currentId);
	else {
		extant_path = newFileDialog();
		if (extant_path.empty()) return false;
		writeEmptyFile(extant_path);
		s_idBank.associate(extant_path, m_currentId);
		emit pathIdAssociated(extant_path, m_currentId);
	}
	return overwrite(m_currentId);
}

void Document::close(const QUuid& id)
{
	qDebug() << __FUNCTION__;

	m_cache.remove(id);
	if (m_currentId == id)
		m_currentId = QUuid();

	for (const auto& [key, value] : s_idBank.paths())
		qDebug() << Path::toQString(key) << value << Qt::endl;

	s_idBank.remove(id);
	StdFs::remove(tempPath(id));
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

void Document::writeEmptyFile(const StdFsPath& path)
{
	Io::writeFile(path);
}

const QString Document::read(StdFsPath path)
{
	auto document = textDocument(m_currentId, path);
	emit startAutoCacheTimer();
	return document->toPlainText();
}

TextDocument* Document::textDocument(const QUuid& id, StdFsPath path)
{
	auto document = m_cache.document(id);
	if (!document)
		document = create(id, path);
	return document;
}

void Document::tempSave(const QUuid& id, const QString& text)
{
	Io::writeFile(tempPath(id), text);
}

Document::StdFsPath Document::tempPath(const QUuid& id)
{
	return m_tempFolder / Path::toStdFs(id.toString() + ".txt~");
}

void Document::backup(const QUuid& id)
{
	auto extant_path = s_idBank.path(id);
	auto backup_path = backupPath(extant_path);
	Path::copy(extant_path, backup_path);
}

Document::StdFsPath Document::backupPath(const StdFsPath& path)
{
	auto name = Path::qStringName(path);
	name += "---" + StringTools::time();
	name = StringTools::removeForbidden(name);
	return m_backupFolder / Path::toStdFs(name + ".bak");
}

bool Document::overwrite(const QUuid& id)
{
	auto path = s_idBank.path(id);
	auto temp_path = tempPath(id);
	if (!Path::areValid(path, temp_path) ||
		!Path::move(temp_path, path, true))
		return false;

	m_cache.remove(m_currentId);
	auto document = textDocument(m_currentId, path);
	emit editedStateChanged(m_currentId, document->edited());

	return true;
}

TextDocument* Document::create(const QUuid& id, StdFsPath path)
{
	QString initial_text;
	QString original_text;

	if (wasEvicted(id))
		recover(id, initial_text, original_text);
	else if (!path.empty())
		Io::toStrings(path, initial_text, original_text);

	auto document = new TextDocument(initial_text, original_text);
	document->setEdited(initial_text != original_text);
	m_cache.insertDocument(id, document);

	return document;
}

bool Document::wasEvicted(const QUuid& id)
{
	if (!s_idBank.contains(id)) return false;
	return StdFs::exists(tempPath(id));
}

void Document::recover(const QUuid& id, QString& initialText, QString& originalText)
{
	qDebug() << __FUNCTION__;

	auto temp_path = tempPath(id);
	if (StdFs::exists(temp_path))
		initialText = Io::readFile(temp_path);

	auto extant_path = s_idBank.path(id);
	if (Path::isValid(extant_path))
		originalText = Io::readFile(extant_path);

	// handle deleted original
	// file system watcher
}
