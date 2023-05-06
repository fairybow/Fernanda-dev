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

	m_scrollUp->setObjectName(name + QString("-scroll-button-up"));
	m_scrollPrevious->setObjectName(name + QString("-scroll-button-previous"));
	m_scrollNext->setObjectName(name + QString("-scroll-button-next"));
	m_scrollDown->setObjectName(name + QString("-scroll-button-down"));

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

void Editor::buildScrollBar()
{
	m_trueEditor->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	m_trueEditor->addScrollBarWidget(m_scrollUp, Qt::AlignTop);
	m_trueEditor->addScrollBarWidget(m_scrollPrevious, Qt::AlignTop);
	m_trueEditor->addScrollBarWidget(m_scrollNext, Qt::AlignBottom);
	m_trueEditor->addScrollBarWidget(m_scrollDown, Qt::AlignBottom);
	for (auto& button : { m_scrollUp, m_scrollDown }) {
		button->setAutoRepeat(true);
		button->setAutoRepeatDelay(500);
	}
	auto up_arrow = "\U000025B2";
	auto down_arrow = "\U000025BC";
	m_scrollUp->setText(up_arrow);
	m_scrollPrevious->setText(up_arrow);
	m_scrollNext->setText(down_arrow);
	m_scrollDown->setText(down_arrow);
	for (auto& button : { m_scrollUp, m_scrollPrevious, m_scrollNext, m_scrollDown })
		button->setMinimumHeight(30);
	scrollButtonEnabler();
}

void Editor::connections()
{
	connectMultiple(m_trueEditor->verticalScrollBar(), this, [&] {
		scrollButtonEnabler();
		}, &QScrollBar::rangeChanged, &QScrollBar::valueChanged);
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

void Editor::scrollButtonEnabler()
{
	m_trueEditor->isMinimumScroll() ? m_scrollUp->setEnabled(false) : m_scrollUp->setEnabled(true);
	m_trueEditor->isMaximumScroll() ? m_scrollDown->setEnabled(false) : m_scrollDown->setEnabled(true);
}
