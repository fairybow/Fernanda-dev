#include "Editor.h"

Editor::Editor(QWidget* parent)
	: QPlainTextEdit(parent)
{
	connect(this, &Editor::cursorPositionChanged, this, [&] { typewriter(); });
	connect(this, &Editor::textChanged, this, [&] { typewriter(); });
}

bool Editor::isTypewriter() const
{
	return m_isTypewriter;
}

void Editor::setIsTypewriter(bool isTypewriter)
{
	m_isTypewriter = isTypewriter;

	if (isTypewriter) {
		m_previousCos = centerOnScroll();
		setCenterOnScroll(true);
	}
	else
		setCenterOnScroll(m_previousCos);

	typewriter();
}

void Editor::typewriter()
{
	if (m_isTypewriter)
		centerCursor();
}
