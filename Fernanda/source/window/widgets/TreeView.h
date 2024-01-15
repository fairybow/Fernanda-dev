#pragma once

#include "../../common/Path.hpp"

#include <QDebug>
#include <QFileSystemModel>
#include <QModelIndex>
#include <QTreeView>

class TreeView : public QTreeView
{
	Q_OBJECT

public:
	TreeView(QWidget* parent = nullptr);
	~TreeView() { qDebug() << __FUNCTION__; }

private:
	QFileSystemModel* m_fileSystemModel = new QFileSystemModel(this);

	void setup();
	void connections();
	void setDefaultRoot(); //<- Temporary.

private slots:
	void onClicked(const QModelIndex& index);
	void onDoubleClicked(const QModelIndex& index);

signals:
	void fileClicked(const Path& path);
	void fileDoubleClicked(const Path& path);
	void folderClicked(const Path& path);
	void folderDoubleClicked(const Path& path);
};
