#include "../../common/Fx.hpp"
#include "../../common/Layout.hpp"
#include "../../common/StringTools.hpp"
#include "Meter.h"

#include <QChar>
#include <QList>
#include <QRegularExpression>
#include <QString>
#include <QTextCursor>
#include <QTextDocumentFragment>

constexpr char LINE_POS_LABEL[] = "line ";
constexpr char LINE_POS_LABEL_SHORT[] = "ln ";
constexpr char COL_POS_LABEL[] = "column ";
constexpr char COL_POS_LABEL_SHORT[] = "col ";
constexpr char LINES_LABEL[] = " lines";
constexpr char WORDS_LABEL[] = " words";
constexpr char CHARS_LABEL[] = " characters";
constexpr char CHARS_LABEL_SHORT[] = " chars";
constexpr char SEPARATOR[] = " / ";
constexpr char JOINER[] = ", ";

Meter::Meter(QWidget* parent, int autoCountCharLimit)
	: QWidget(parent), m_autoCountCharLimit(autoCountCharLimit)
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

int Meter::autoCountCharLimit() const
{
	return m_autoCountCharLimit;
}

void Meter::setAutoCountCharLimit(int limit)
{
	m_autoCountCharLimit = limit;

	run();
}

bool Meter::hasPositionLabels() const
{
	return m_hasPositionLabels;
}

void Meter::setHasPositionLabels(bool has)
{
	m_hasPositionLabels = has;

	run();
}

bool Meter::hasCountLabels() const
{
	return m_hasCountLabels;
}

void Meter::setHasCountLabels(bool has)
{
	m_hasCountLabels = has;

	run();
}

bool Meter::useShortLabels() const
{
	return m_useShortLabels;
}

void Meter::setUseShortLabels(bool use)
{
	m_useShortLabels = use;

	run();
}

bool Meter::hasLinePosition() const
{
	return m_hasLinePosition;
}

void Meter::setHasLinePosition(bool has)
{
	m_hasLinePosition = has;

	run();
}

bool Meter::hasColumnPosition() const
{
	return m_hasColumnPosition;
}

void Meter::setHasColumnPosition(bool has)
{
	m_hasColumnPosition = has;

	run();
}

bool Meter::hasLineCount() const
{
	return m_hasLineCount;
}

void Meter::setHasLineCount(bool has)
{
	m_hasLineCount = has;

	run();
}

bool Meter::hasWordCount() const
{
	return m_hasWordCount;
}

void Meter::setHasWordCount(bool has)
{
	m_hasWordCount = has;

	run();
}

bool Meter::hasCharCount() const
{
	return m_hasCharCount;
}

void Meter::setHasCharCount(bool has)
{
	m_hasCharCount = has;

	run();
}

void Meter::run()
{
	updatePositions();
	updateCounts(Force::Yes);
}

void Meter::reset()
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
	QWidgetList widgets{ m_positions, m_separator, m_counts, m_refreshCounts };
	Layout::box(Layout::Box::Horizontal, this, widgets);

	m_separator->setText(SEPARATOR);
	hideAll();

	Fx::opacify(m_positions, 0.8);
	Fx::opacify(m_separator, 0.3);
	Fx::opacify(m_counts, 0.8);

	connect(m_refreshCounts, &UiButton::clicked, this, &Meter::onRefreshCountsClicked);
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
	if (!m_currentEditor) return;

	auto is_needed = hasAnyPosition();
	maybeShowSubWidget(m_positions, is_needed);

	if (is_needed)
		m_positions->setText(positions());
}

void Meter::updateCounts(Force force)
{
	if (!m_currentEditor || (!m_autoCount && force == Force::No)) return;

	auto is_needed = hasAnyCount();
	maybeShowSubWidget(m_counts, is_needed);

	if (is_needed)
		m_counts->setText(counts());

	maybeToggleRefreshCounts();
}

QString Meter::positions()
{
	QStringList elements;
	auto cursor = m_currentEditor->textCursor();

	if (m_hasLinePosition)
		elements << label(Label::LinePos) + QString::number(cursor.blockNumber() + 1);

	if (m_hasColumnPosition)
		elements << label(Label::ColPos) + QString::number(cursor.positionInBlock() + 1);

	return elements.join(JOINER);
}

QString Meter::counts()
{
	QStringList elements;
	auto cursor = m_currentEditor->textCursor();

	auto is_selection = cursor.hasSelection();
	auto text = is_selection
		? cursor.selectedText()
		: m_currentEditor->toPlainText();

	if (m_hasLineCount) {
		auto block_count = is_selection
			? selectedLineCount()
			: m_currentEditor->blockCount();

		elements << QString::number(block_count) + label(Label::LineCount);
	}

	if (m_hasWordCount)
		elements << StringTools::wordCountString(text) + label(Label::WordCount);

	auto char_count = text.count();

	if (m_hasCharCount)
		elements << QString::number(char_count) + label(Label::CharCount);

	maybeToggleAutoCount(char_count);

	return elements.join(JOINER);
}

QString Meter::label(Label type) const
{
	QString text;
	auto blank = "";

	switch (type) {
	case Label::CharCount:
		text = m_hasCountLabels ? m_useShortLabels ? CHARS_LABEL_SHORT : CHARS_LABEL : blank;
		break;
	case Label::ColPos:
		text = m_hasPositionLabels ? m_useShortLabels ? COL_POS_LABEL_SHORT : COL_POS_LABEL : blank;
		break;
	case Label::LineCount:
		text = m_hasCountLabels ? LINES_LABEL : blank;
		break;
	case Label::LinePos:
		text = m_hasPositionLabels ? m_useShortLabels ? LINE_POS_LABEL_SHORT : LINE_POS_LABEL : blank;
		break;
	case Label::WordCount:
		text = m_hasCountLabels ? WORDS_LABEL : blank;
		break;
	}

	return text;
}

void Meter::maybeShowSubWidget(QLabel* label, bool show)
{
	label->setVisible(show);

	m_separator->setVisible(m_positions->isVisible() && m_counts->isVisible());
}

void Meter::maybeToggleAutoCount(int characters)
{
	m_autoCount = (characters <= m_autoCountCharLimit);
}

void Meter::maybeToggleRefreshCounts()
{
	m_refreshCounts->setVisible(!m_autoCount && hasAnyCount());
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
	auto widgets = QWidgetList{ m_positions, m_counts, m_separator, m_refreshCounts };

	for (auto& widget : widgets)
		widget->hide();
}

void Meter::onRefreshCountsClicked()
{
	updateCounts(Force::Yes);

	m_currentEditor->setFocus();
}

void Meter::onSelectionChanged()
{
	static QTextDocumentFragment previous_selection;

	auto selection = m_currentEditor->textCursor().selection();

	if (selection.toPlainText() == previous_selection.toPlainText()) return;

	// Note:
	// If `!m_autoCount`, then delay recalculating on selection release?
	// I think performance spike and issues may be inevitable with very large texts,
	// but this function mitigates the issues that arise with `QPlainTextEdit`'s 
	// perhaps overly-frequent emissions of `selectionChanged()`.

	updateCounts(Force::Yes);
	previous_selection = selection;
}
