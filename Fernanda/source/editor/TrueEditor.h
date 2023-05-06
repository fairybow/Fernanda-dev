#pragma once

#include "LineNumberArea.h"

#include <QFont>
#include <QPainter>
#include <QPlainTextEdit>
#include <QRegularExpressionMatchIterator>
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
	void setCursorStyle(const QString& styleSheet);

	inline bool isMaximumScroll() const { return (verticalScrollBar()->sliderPosition() == verticalScrollBar()->maximum()); }
	inline bool isMinimumScroll() const { return (verticalScrollBar()->sliderPosition() == verticalScrollBar()->minimum()); }

signals:
	bool getHasLineHighlight();
	bool getHasCursorBlock();
	bool getHasCursorBlink();
	void startCursorBlinkTimer();

protected:
	virtual void paintEvent(QPaintEvent* event) override;
	virtual void resizeEvent(QResizeEvent* event) override;

private:
	bool m_cursorBlinkVisible = true;
	LineNumberArea* m_lineNumberArea;
	QTimer* m_cursorBlinkTimer = new QTimer(this);
	QString m_cursorColorHex;
	QString m_cursorUnderColorHex;

	void connections();
	void highlightCurrentLine();
	void updateLineNumberArea(const QRect& rect, int dy);
	const QColor highlight();
	const QChar currentChar();
	const QRect reshapeCursor(const QChar& currentChar);
	const QColor recolorCursor(bool under = false);

	void updateLineNumberAreaWidth() { setViewportMargins(lineNumberAreaWidth(), 0, 0, 0); }
};
