#pragma once

#include "../common/Event.hpp"
#include "BlockCursor.hpp"
#include "LineNumberArea.h"

#include <QColor>
#include <QFont>
#include <QPainter>
#include <QPlainTextEdit>
#include <QRegularExpressionMatchIterator>
#include <QScrollBar>
#include <QTextBlock>

class TrueEditor : public QPlainTextEdit
{
	Q_OBJECT

public:
	TrueEditor(QWidget* parent = nullptr);

	void lineNumberAreaPaintEvent(QPaintEvent* event);
	int lineNumberAreaWidth();
	void setFont(const QFont& font);
	void setLineNumberArea(LineNumberArea* lineNumberArea);
	void setCursorStyle(const QString& styleSheet);

signals:
	bool getHasLineHighlight();
	bool getHasCursorBlock();
	bool getHasCursorBlink();

protected:
	virtual void paintEvent(QPaintEvent* event) override;
	virtual void resizeEvent(QResizeEvent* event) override;

private:
	BlockCursor* m_cursor = new BlockCursor(this);
	LineNumberArea* m_lineNumberArea;

	void connections();
	void cursorConnections();
	void highlightCurrentLine();
	void updateLineNumberArea(const QRect& rect, int dy);
	const QColor highlight();

	void updateLineNumberAreaWidth() { setViewportMargins(lineNumberAreaWidth(), 0, 0, 0); }
};
