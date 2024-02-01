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

constexpr char LINE_POS_LABEL[] = "line";
constexpr char COL_POS_LABEL[] = "column";
constexpr char LINES_LABEL[] = "lines";
constexpr char WORDS_LABEL[] = "words";
constexpr char CHARS_LABEL[] = "characters";
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
	setMember(m_autoCountCharLimit, limit, Run::Yes);
}

bool Meter::hasPositionLabels() const
{
	return m_hasPositionLabels;
}

void Meter::setHasPositionLabels(bool has)
{
	setMember(m_hasPositionLabels, has);
}

bool Meter::hasCountLabels() const
{
	return m_hasCountLabels;
}

void Meter::setHasCountLabels(bool has)
{
	setMember(m_hasCountLabels, has);
}

bool Meter::hasLinePosition() const
{
	return m_hasLinePosition;
}

void Meter::setHasLinePosition(bool has)
{
	setMember(m_hasLinePosition, has, Run::Yes);
}

bool Meter::hasColumnPosition() const
{
	return m_hasColumnPosition;
}

void Meter::setHasColumnPosition(bool has)
{
	setMember(m_hasColumnPosition, has, Run::Yes);
}

bool Meter::hasLineCount() const
{
	return m_hasLineCount;
}

void Meter::setHasLineCount(bool has)
{
	setMember(m_hasLineCount, has, Run::Yes);
}

bool Meter::hasWordCount() const
{
	return m_hasWordCount;
}

void Meter::setHasWordCount(bool has)
{
	setMember(m_hasWordCount, has, Run::Yes);
}

bool Meter::hasCharCount() const
{
	return m_hasCharCount;
}

void Meter::setHasCharCount(bool has)
{
	setMember(m_hasCharCount, has, Run::Yes);
}

QString Meter::linePositionLabel() const
{
	return m_linePositionLabel;
}

void Meter::setLinePositionLabel(const QString& text)
{
	setMember(m_linePositionLabel, text);
}

QString Meter::columnPositionLabel() const
{
	return m_columnPositionLabel;
}

void Meter::setColumnPositionLabel(const QString& text)
{
	setMember(m_columnPositionLabel, text);
}

QString Meter::lineCountLabel() const
{
	return m_lineCountLabel;
}

void Meter::setLineCountLabel(const QString& text)
{
	setMember(m_lineCountLabel, text);
}

QString Meter::wordCountLabel() const
{
	return m_wordCountLabel;
}

void Meter::setWordCountLabel(const QString& text)
{
	setMember(m_wordCountLabel, text);
}

QString Meter::charCountLabel() const
{
	return m_charCountLabel;
}

void Meter::setCharCountLabel(const QString& text)
{
	setMember(m_charCountLabel, text);
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

	switch (type) {
	case Label::CharCount:
		text = labelText(m_hasCountLabels, m_charCountLabel, CHARS_LABEL);
		break;
	case Label::ColPos:
		text = labelText(m_hasPositionLabels, m_columnPositionLabel, COL_POS_LABEL);
		break;
	case Label::LineCount:
		text = labelText(m_hasCountLabels, m_lineCountLabel, LINES_LABEL);
		break;
	case Label::LinePos:
		text = labelText(m_hasPositionLabels, m_linePositionLabel, LINE_POS_LABEL);
		break;
	case Label::WordCount:
		text = labelText(m_hasCountLabels, m_wordCountLabel, WORDS_LABEL);
		break;
	}

	if (!text.isEmpty()) {
		(type == Label::ColPos || type == Label::LinePos)
			? text += " "
			: text = " " + text;
	}

	return text;
}

QString Meter::labelText(bool has, const QString& customLabel, const QString& defaultLabel) const
{
	return has
		? (customLabel.isEmpty()
			? defaultLabel
			: customLabel)
		: QString();
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
