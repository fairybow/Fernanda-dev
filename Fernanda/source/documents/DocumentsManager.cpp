#include "DocumentsManager.h"

IdBank DocumentsManager::s_idBank;

DocumentsManager::DocumentsManager(
	const Folders& folders,
	QObject* parent,
	int cacheMaxCost)
	: QObject(parent),
	m_userFolder(folders.user),
	m_tempFolder(folders.temp),
	m_backupFolder(folders.backup)
{
	DocumentsCache::setMaxCost(cacheMaxCost);
}

DocumentsManager::StdFsPath DocumentsManager::newFileDialog(QWidget* parent, const QString& name/*<-- unused atm*/)
{
	auto file_name = Path::toStdFs(name.left(30));
	auto path = QFileDialog::getSaveFileName(
		parent, tr("Create a new file..."),
		Path::toQString(m_userFolder / file_name), tr(DIALOG_FILE_TYPE));

	return Path::toStdFs(path);
}

DocumentsManager::StdFsPath DocumentsManager::openFileDialog(QWidget* parent)
{
	auto path = QFileDialog::getOpenFileName(
		parent, tr("Open an existing file..."),
		Path::toQString(m_userFolder), tr(DIALOG_FILE_TYPE));

	return Path::toStdFs(path);
}

Document* DocumentsManager::setActive(const QUuid& id)
{
	m_activeId = id;
	return active();
}

QUuid DocumentsManager::newUnsaved()
{
	auto id = s_idBank.recordNew();
	create(id);
	return id;
}

QUuid DocumentsManager::fromDisk(PathType type, const StdFsPath& path)
{
	qDebug() << __FUNCTION__;

	QUuid id = s_idBank.fromPath(path);
	qDebug() << id << path;

	if (type == PathType::New) {
		qDebug() << "is new";
		writeEmptyFile(path);
	}

	create(id, path);
		
	return id;
}

Document* DocumentsManager::retrieve(const QUuid& id, const StdFsPath& path)
{
	auto& cache = DocsCache::instance();
	auto document = cache.document(id);
	if (!document)
		document = newDocument(id, path);

	return document;
}

Document* DocumentsManager::newDocument(const QUuid& id, const StdFsPath& path)
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

bool DocumentsManager::wasEvicted(const QUuid& id)
{
	if (!s_idBank.contains(id)) return false;
	return StdFs::exists(tempPath(id));
}

DocumentsManager::StdFsPath DocumentsManager::tempPath(const QUuid& id)
{
	return m_tempFolder / Path::toStdFs(id.toString() + FILE_TYPE + "~");
}

void DocumentsManager::recover(const QUuid& id, QString& initialText, QString& originalText, QString& title)
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

bool DocumentsManager::writeEmptyFile(const StdFsPath& path)
{
	return Io::writeFile(path);
}
