#include "Editor.h"

Editor::Editor(QWidget* parent)
	: QPlainTextEdit(parent)
{
	connect(this, &Editor::cursorPositionChanged, this, &Editor::typewriter);
	connect(this, &Editor::textChanged, this, &Editor::typewriter);
}

bool Editor::isTypewriter() const
{
	return m_isTypewriter;
}

void Editor::setIsTypewriter(bool isTypewriter)
{
	m_isTypewriter = isTypewriter;

	typewriter();
}

void Editor::typewriter()
{
	if (m_isTypewriter)
		centerCursor();
}
