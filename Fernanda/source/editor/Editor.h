#pragma once

#include "../common/Fx.hpp"
#include "../common/Io.hpp"
#include "../common/Layout.hpp"
#include "../common/Widget.hpp"
#include "TrueEditor.h"
// potentially move block cursor and lna here and manage
// the interactions between them with this container class

#include <QFont>
#include <QLabel>
#include <QVector>

class Editor : public Widget<>
{
	Q_OBJECT

public:
	Editor(const char* name, const QFont& defaultFont = QFont(), QWidget* parent = nullptr);

	inline void setFocus() { m_trueEditor->setFocus(); }

	inline QFont defaulFont() const { return m_defaultFont; }
	inline void setFont(const QFont& font) { m_trueEditor->setFont(font); }
	inline void setReadOnly(bool readOnly) { m_trueEditor->setReadOnly(readOnly); }
	inline bool isReadOnly() const { return m_trueEditor->isReadOnly(); }
	inline QString selectedText() const { return m_trueEditor->textCursor().selectedText(); }
	inline int selectedLineCount() const { return m_trueEditor->selectedLineCount(); }
	inline QString toPlainText() const { return m_trueEditor->toPlainText(); }
	inline int blockCount() const { return m_trueEditor->blockCount(); }
	inline int cursorBlockNumber() const { return m_trueEditor->textCursor().blockNumber(); }
	inline int cursorPositionInBlock() const { return m_trueEditor->textCursor().positionInBlock(); }
	inline bool hasSelection() const { return m_trueEditor->textCursor().hasSelection(); }

signals:
	void selectionChanged();
	void textChanged();
	void cursorPositionChanged();

protected:
	virtual void changeEvent(QEvent* event) override;

private:
	const QFont m_defaultFont;
	bool m_hasShadow = true;
	bool m_hasLineHighlight = true;
	bool m_hasCursorBlink = true;
	bool m_hasCursorBlock = true;
	bool m_hasCursorEnsureVisible = true;
	bool m_hasCursorTypewriter = true;
	TrueEditor* m_trueEditor = new TrueEditor(this);
	LineNumberArea* m_lineNumberArea = new LineNumberArea(m_trueEditor);
	QLabel* m_shadow = new QLabel(this);
	QLabel* m_overlay = new QLabel(this);
	QLabel* m_underlay = new QLabel(this);

	void nameObjects(const char* name);
	void setupTrueEditor();
	void setupShadow();
	void buildScrollBar();
	void connections();
	void trueEditorConnections();
	//void cursorConnections();
	//void lineNumberAreaConnections();
};
