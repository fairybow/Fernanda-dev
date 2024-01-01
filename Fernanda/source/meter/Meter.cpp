#include "../common/Fx.hpp"
#include "../common/Layout.hpp"
#include "Meter.h"

#include <QChar>
#include <QList>
#include <QRegularExpression>
#include <QString>
#include <QTextCursor>
#include <QTextDocumentFragment>

constexpr char LINE_POS_LABEL[] = "ln ";
constexpr char COL_POS_LABEL[] = "col ";
constexpr char LINES_LABEL[] = " lines";
constexpr char WORDS_LABEL[] = " words";
constexpr char CHARS_LABEL[] = " chars";
constexpr char SEPARATOR[] = " / ";
constexpr char JOINER[] = ", ";
constexpr char CAPTURE_LEADING_WHITESPACE[] = "(\\s|\\n|\\r|\U00002029|^)+";

Meter::Meter(QWidget* parent)
	: QWidget(parent)
{
	setup();
}

QPlainTextEdit* Meter::currentEditor() const
{
	return m_currentEditor;
}

void Meter::setCurrentEditor(QPlainTextEdit* editor)
{
	if (m_currentEditor)
		disconnectEditor();

	m_currentEditor = editor;

	connectEditor();

	run();
}

void Meter::run()
{
	updatePositions();
	updateCounts(Force::Yes);
}

void Meter::clear()
{
	if (m_currentEditor)
		disconnectEditor();

	m_currentEditor = nullptr;

	hideAll();
}

bool Meter::hasAnyPosition() const
{
	return m_hasLinePosition || m_hasColumnPosition;
}

bool Meter::hasAnyCount() const
{
	return m_hasLineCount || m_hasWordCount || m_hasCharCount;
}

void Meter::setup()
{
	QWidgetList widgets{ m_positions, m_separator, m_counts, m_refresh };
	Layout::box(Box::Horizontal, this, widgets);

	m_separator->setText(SEPARATOR);
	hideAll();

	Fx::opacify(m_positions, 0.8);
	Fx::opacify(m_separator, 0.3);
	Fx::opacify(m_counts, 0.8);

	connect(m_refresh, &UiButton::clicked, this, &Meter::onRefreshClicked);
}

void Meter::connectEditor()
{
	connect(m_currentEditor, &QPlainTextEdit::cursorPositionChanged, this, [&] { updatePositions(); });
	connect(m_currentEditor, &QPlainTextEdit::textChanged, this, [&] { updateCounts(); });
	connect(m_currentEditor, &QPlainTextEdit::selectionChanged, this, &Meter::onSelectionChanged);
}

void Meter::disconnectEditor()
{
	disconnect(m_currentEditor, nullptr, this, nullptr);
}

void Meter::updatePositions()
{
	auto is_needed = hasAnyPosition();
	maybeShowLabel(m_positions, is_needed);

	if (!is_needed) return;

	auto cursor = m_currentEditor->textCursor();

	QStringList elements;

	if (m_hasLinePosition)
		elements << LINE_POS_LABEL + QString::number(cursor.blockNumber() + 1);

	if (m_hasColumnPosition)
		elements << COL_POS_LABEL + QString::number(cursor.positionInBlock() + 1);

	m_positions->setText(elements.join(JOINER));
}

void Meter::updateCounts(Force force)
{
	if (!m_autoCount && force == Force::No) return;

	auto is_needed = hasAnyCount();
	maybeShowLabel(m_counts, is_needed);

	if (!is_needed) return;

	auto cursor = m_currentEditor->textCursor();

	QStringList elements;

	auto is_selection = cursor.hasSelection();
	auto text = is_selection
		? cursor.selectedText()
		: m_currentEditor->toPlainText();

	if (m_hasLineCount) {
		auto block_count = is_selection
			? selectedLineCount()
			: m_currentEditor->blockCount();
		elements << QString::number(block_count) + LINES_LABEL;
	}

	if (m_hasWordCount) {
		auto regex = QRegularExpression(CAPTURE_LEADING_WHITESPACE);
		auto words = text.split(regex, Qt::SkipEmptyParts);
		elements << QString::number(words.count()) + WORDS_LABEL;
	}

	auto char_count = text.count();

	if (m_hasCharCount)
		elements << QString::number(text.count()) + CHARS_LABEL;

	m_counts->setText(elements.join(JOINER));

	m_autoCount = (char_count <= 15000);
	maybeShowRefresh();
}

void Meter::maybeShowLabel(QLabel* label, bool show)
{
	label->setVisible(show);

	maybeShowSeparator();
}

void Meter::maybeShowSeparator()
{
	m_separator->setVisible(m_positions->isVisible() && m_counts->isVisible());
}

void Meter::maybeShowRefresh()
{
	m_refresh->setVisible(!m_autoCount && hasAnyCount());
}

int Meter::selectedLineCount() const
{
	auto cursor = m_currentEditor->textCursor();
	if (!cursor.hasSelection()) return 1;

	auto text = cursor.selectedText();

	return text.count(QChar(0x2029)) + 1;
}

void Meter::hideAll()
{
	auto widgets = QWidgetList{ m_positions, m_counts, m_separator, m_refresh };

	for (auto& widget : widgets)
		widget->hide();
}

void Meter::onRefreshClicked()
{
	updateCounts(Force::Yes);

	m_currentEditor->setFocus();
}

void Meter::onSelectionChanged()
{
	static QTextDocumentFragment previous_selection;

	auto selection = m_currentEditor->textCursor().selection();

	if (selection.toPlainText() == previous_selection.toPlainText()) return;

	// If `!m_autoCount`, then delay recalculating on selection release?
	// I think performance spike and issues may be inevitable with very large texts,
	// but this function mitigates the issues that arise with `QPlainTextEdit`'s 
	// perhaps overly-frequent emissions of `selectionChanged()`.

	updateCounts(Force::Yes);
	previous_selection = selection;
}
