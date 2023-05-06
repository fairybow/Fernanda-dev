#pragma once

#include "LineNumberArea.h"

#include <QFont>
#include <QPainter>
#include <QPlainTextEdit>
#include <QScrollBar>
#include <QTextBlock>
#include <QTimer>

class TrueEditor : public QPlainTextEdit
{
	Q_OBJECT

public:
	TrueEditor(QWidget* parent = nullptr);

	void lineNumberAreaPaintEvent(QPaintEvent* event);
	int lineNumberAreaWidth();
	void setFont(const QFont& font);
	void setLineNumberArea(LineNumberArea* lineNumberArea);

	inline bool isMaximumScroll() const { return (verticalScrollBar()->sliderPosition() == verticalScrollBar()->maximum()); }
	inline bool isMinimumScroll() const { return (verticalScrollBar()->sliderPosition() == verticalScrollBar()->minimum()); }

signals:
	bool getHasLineHighlight();

protected:
	virtual void paintEvent(QPaintEvent* event) override;
	virtual void resizeEvent(QResizeEvent* event) override;

private:
	LineNumberArea* m_lineNumberArea;

	void highlightCurrentLine();
	void updateLineNumberArea(const QRect& rect, int dy);
	const QColor highlight();

	void updateLineNumberAreaWidth() { setViewportMargins(lineNumberAreaWidth(), 0, 0, 0); }
};
