#pragma once

#include <QPainter>
#include <QSplitterHandle>

class SplitterHandle : public QSplitterHandle
{
public:
	inline SplitterHandle(Qt::Orientation orientation, QSplitter* parent) : QSplitterHandle(orientation, parent) {}

	inline virtual QSize sizeHint() const override
	{
		QSize size = QSplitterHandle::sizeHint();
		auto i = 5;
		(orientation() != Qt::Horizontal) ? size.setHeight(i) : size.setWidth(i);
		return size;
	}
};
