#pragma once

#include "StyledItemDelegate.hpp"

#include <QStandardItemModel>
#include <QTreeView>

class Tree : public QTreeView
{
public:
	Tree(const char* name, QWidget* parent = nullptr)
		: QTreeView(parent)
	{
		setObjectName(name);
		setItemDelegate(m_delegate);
		setModel(m_itemModel);
	}

private:
	QStandardItemModel* m_itemModel = new QStandardItemModel(this);
	StyledItemDelegate* m_delegate = new StyledItemDelegate(this);
};
