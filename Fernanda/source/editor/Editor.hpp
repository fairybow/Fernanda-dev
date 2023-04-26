#pragma once

#include "../common/Layout.hpp"
#include "TrueEditor.h"

#include <QVector>
#include <QWidget>

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
		Layout::setStack(this, { m_trueEditor }); // shadow, overlay, trueEditor, underlay
	}

private:
	TrueEditor* m_trueEditor = new TrueEditor(this);
	LineNumberArea* m_lineNumberArea = new LineNumberArea(m_trueEditor);
};
