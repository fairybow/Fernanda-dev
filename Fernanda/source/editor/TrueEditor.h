#pragma once

#include "LineNumberArea.h"

#include <QPainter>
#include <QPlainTextEdit>
#include <QScrollBar>
#include <QTextBlock>

class TrueEditor : public QPlainTextEdit
{
	Q_OBJECT

public:
	TrueEditor(QWidget* parent = nullptr);

	void setLineNumberArea(LineNumberArea* lineNumberArea);
	void lineNumberAreaPaintEvent(QPaintEvent* event);
	int lineNumberAreaWidth();

	inline bool isMaximumScroll() const { return (verticalScrollBar()->sliderPosition() == verticalScrollBar()->maximum()); }
	inline bool isMinimumScroll() const { return (verticalScrollBar()->sliderPosition() == verticalScrollBar()->minimum()); }

protected:
	virtual void resizeEvent(QResizeEvent* event) override;

private:
	LineNumberArea* m_lineNumberArea;

	void updateLineNumberAreaWidth(int newBlockCount);
	void highlightCurrentLine();
	void updateLineNumberArea(const QRect& rect, int dy);
};
