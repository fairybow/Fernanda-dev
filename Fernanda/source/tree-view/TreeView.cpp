#include "TreeView.h"

#include <QStandardPaths> //<- Temporary.

TreeView::TreeView(QWidget* parent)
	: QTreeView(parent)
{
	setup();
}

void TreeView::setup()
{
	setModel(m_fileSystemModel);
	setDefaultRoot(); //<- Temporary.

	connections();
}

void TreeView::connections()
{
	connect(this, &QTreeView::clicked, this, &TreeView::onClicked);
	connect(this, &QTreeView::doubleClicked, this, &TreeView::onDoubleClicked);
}

void TreeView::setDefaultRoot()
{
	auto root_path = QStandardPaths::locate(QStandardPaths::DocumentsLocation, nullptr, QStandardPaths::LocateDirectory);
	root_path += "/Fernanda";
	//^ Get `root_path` from settings.

	auto root_index = m_fileSystemModel->setRootPath(root_path);
	setRootIndex(root_index);
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
