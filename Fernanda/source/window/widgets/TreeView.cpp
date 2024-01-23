#include "TreeView.h"

TreeView::TreeView(QWidget* parent)
	: QTreeView(parent)
{
	setup();
}

void TreeView::setRoot(const Path& path)
{
	auto root_index = m_fileSystemModel->setRootPath(path.toQString());
	setRootIndex(root_index);
}

void TreeView::setup()
{
	setModel(m_fileSystemModel);

	connect(this, &QTreeView::clicked, this, &TreeView::onClicked);
	connect(this, &QTreeView::doubleClicked, this, &TreeView::onDoubleClicked);
}

void TreeView::onClicked(const QModelIndex& index)
{
	auto path = Path(m_fileSystemModel->filePath(index));

	if (path.isFile())
		emit fileClicked(path);
	else if (path.isFolder())
		emit folderClicked(path);
}

void TreeView::onDoubleClicked(const QModelIndex& index)
{
	auto path = Path(m_fileSystemModel->filePath(index));

	if (path.isFile())
		emit fileDoubleClicked(path);
	else if (path.isFolder())
		emit folderDoubleClicked(path);
}
