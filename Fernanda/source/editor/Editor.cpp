#include "Editor.h"

Editor::Editor(const char* name, const QFont& defaultFont, QWidget* parent)
	: Widget(name, parent), m_defaultFont(defaultFont)
{
	nameObjects(name);
	setupTrueEditor();
	setupShadow();
	buildScrollBar();
	connections();
	Layout::transpareForMouse({ m_shadow, m_overlay });
	Layout::stack({ m_shadow, m_overlay, m_trueEditor, m_underlay}, this);

	// testing

	m_overlay->hide();
}

void Editor::changeEvent(QEvent* event)
{
	if (event->type() == QEvent::StyleChange)
		m_trueEditor->setCursorStyle(styleSheet());
	QWidget::changeEvent(event);
}

void Editor::nameObjects(const char* name)
{
	m_trueEditor->setObjectName(name);
	m_lineNumberArea->setObjectName(name + QString("-line-number-area"));
	m_shadow->setObjectName(name + QString("-shadow"));
	m_overlay->setObjectName(name + QString("-overlay"));
	m_underlay->setObjectName(name + QString("-underlay"));
}

void Editor::setupTrueEditor()
{
	//m_trueEditor->setReadOnly(true);
	m_trueEditor->setLineNumberArea(m_lineNumberArea);
	//m_trueEditor->viewport()->setCursor(Qt::ArrowCursor);
}

void Editor::setupShadow()
{
	m_shadow->setStyleSheet(Io::readFile(":/editor/Shadow.qss"));
	m_shadow->setGraphicsEffect(Fx::blur(15, m_shadow));
}

void Editor::buildScrollBar()
{
	//
}

void Editor::connections()
{
	cursorConnections();
	lineNumberAreaConnections();
	trueEditorConnections();
}

void Editor::cursorConnections()
{
	//
}

void Editor::lineNumberAreaConnections()
{
	//
}

void Editor::trueEditorConnections()
{
	connect(m_trueEditor, &TrueEditor::getHasLineHighlight, this, [&] {
		return m_hasLineHighlight;
		});
	connect(m_trueEditor, &TrueEditor::getHasCursorBlink, this, [&] {
		return m_hasCursorBlink;
		});
	connect(m_trueEditor, &TrueEditor::getHasCursorBlock, this, [&] {
		return m_hasCursorBlock;
		});
}
