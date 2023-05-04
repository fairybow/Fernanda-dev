#pragma once

#include "../common/Widget.hpp"
#include "StyledItemDelegate.hpp"

#include <QScrollBar>
#include <QStandardItemModel>
#include <QTreeView>

class TreeView : public Widget<QTreeView>
{
public:
	inline TreeView(const char* name, QWidget* parent = nullptr)
		: Widget(name, parent)
	{
		horizontalScrollBar()->setObjectName("HScrollBar"); // rename in qss
		verticalScrollBar()->setObjectName("TreeViewVScrollBar"); // rename in qss
		setItemDelegate(m_delegate);
		setModel(m_itemModel);
	}

private:
	QStandardItemModel* m_itemModel = new QStandardItemModel(this);
	StyledItemDelegate* m_delegate = new StyledItemDelegate(this);
};
