#include "Editor.h"

Editor::Editor(const char* name, const QFont& defaultFont, QWidget* parent)
	: Widget(name, parent), m_defaultFont(defaultFont)
{
	nameObjects(name);
	setupTrueEditor();
	setupShadow();
	buildScrollBar(name);
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

	m_trueEditor->horizontalScrollBar()->setObjectName("HorizontalScrollBar");
	m_trueEditor->verticalScrollBar()->setObjectName("VerticalScrollBar");
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

void Editor::buildScrollBar(const char* name)
{
	auto up = new ScrollButton(m_trueEditor, ScrollButton::Type::Up, this);
	auto previous = new ScrollButton(m_trueEditor, ScrollButton::Type::Previous, this);
	auto next = new ScrollButton(m_trueEditor, ScrollButton::Type::Next, this);
	auto down = new ScrollButton(m_trueEditor, ScrollButton::Type::Down, this);

	up->setObjectName(name + QString("-scroll-button-up"));
	previous->setObjectName(name + QString("-scroll-button-previous"));
	next->setObjectName(name + QString("-scroll-button-next"));
	down->setObjectName(name + QString("-scroll-button-down"));
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
