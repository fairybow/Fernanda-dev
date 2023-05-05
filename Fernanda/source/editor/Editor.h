#pragma once

#include "../common/Io.hpp"
#include "../common/Layout.hpp"
#include "../common/Widget.hpp"
#include "TrueEditor.h"

#include <QGraphicsBlurEffect>
#include <QLabel>
#include <QPushButton>
#include <QScrollBar>
#include <QVector>

class Editor : public Widget<>
{
	Q_OBJECT

public:
	Editor(const char* name, QWidget* parent = nullptr);

	// funnel to and from QPTE
	void setReadOnly(bool readOnly) { m_trueEditor->setReadOnly(readOnly); }
	bool isReadOnly() const { return m_trueEditor->isReadOnly(); }
	// add when needed

private:
	TrueEditor* m_trueEditor = new TrueEditor(this);
	LineNumberArea* m_lineNumberArea = new LineNumberArea(m_trueEditor);
	QLabel* m_shadow = new QLabel(this);
	QLabel* m_overlay = new QLabel(this);
	QLabel* m_underlay = new QLabel(this);
	QPushButton* m_scrollPrevious = new QPushButton;
	QPushButton* m_scrollUp = new QPushButton;
	QPushButton* m_scrollDown = new QPushButton;
	QPushButton* m_scrollNext = new QPushButton;

	void nameObjects(const char* name);
	QGraphicsBlurEffect* blur(int radius = 15);
	void buildScrollBar();
};
