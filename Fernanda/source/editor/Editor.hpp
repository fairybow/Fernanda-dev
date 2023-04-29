#pragma once

#include "../common/Layout.hpp"
#include "TrueEditor.h"

#include <QLabel>
#include <QVector>

class Editor : public QWidget
{
	Q_OBJECT

public:
	inline Editor(const char* name, QWidget* parent = nullptr)
		: QWidget(parent)
	{
		m_trueEditor->setObjectName(name);
		m_lineNumberArea->setObjectName(name + QString("-line-number-area"));
		m_trueEditor->setLineNumberArea(m_lineNumberArea);
		Layout::stack(this, { m_shadow, m_overlay, m_trueEditor, m_underlay });
	}

private:
	TrueEditor* m_trueEditor = new TrueEditor(this);
	LineNumberArea* m_lineNumberArea = new LineNumberArea(m_trueEditor);
	QLabel* m_shadow = new QLabel(this);
	QLabel* m_overlay = new QLabel(this);
	QLabel* m_underlay = new QLabel(this);
};
