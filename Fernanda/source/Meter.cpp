#include "Meter.h"

Meter::Meter(const char* name, QWidget* parent)
	: Widget(name, parent)
{
	for (auto& widget : QWidgetList{ m_positions, m_separator, m_counts })
		widget->setObjectName(name);
	setupLabels();
	connections();
	Layout::box(Layout::Line::Horizontally,
		{ m_positions, m_separator, m_counts, m_refresh }, this);
	updateCounts();
	updatePositions();
}

void Meter::trigger(Type type, bool force)
{
	if ((!m_hasAutoCount && type == Type::Counts) && !force) return;
	switch (type) {
	case Type::Counts:
		updateCounts();
		break;
	case Type::Positions:
		updatePositions();
		break;
	case Type::Selection:
		updateCounts(true);
		break;
	}
}

void Meter::setupLabels()
{
	m_separator->setText("/");
	m_positions->setGraphicsEffect(
		Fx::opacify(0.8, m_positions));
	m_separator->setGraphicsEffect(
		Fx::opacify(0.3, m_separator));
	m_counts->setGraphicsEffect(
		Fx::opacify(0.8, m_counts));
}

void Meter::connections()
{
	connect(this, &Meter::separatorVisibilityCheck, this, [&] {
		(m_positions->isHidden() || m_counts->isHidden())
			? m_separator->hide()
			: m_separator->show();
		});
	connect(this, &Meter::toggleAutoCount, this, [&](bool checked) {
		m_hasAutoCount = checked;
		m_refresh->setVisible(!checked);
		});
	connect(m_refresh, &StatusBarButton::pressed, this, [&] {
		trigger(Type::Counts, true);
		emit editorFocusReturn();
		});
}

void Meter::updateCounts(bool isSelection)
{
	if (!hasAnyCount()) return;
	auto counts_info = emit getCountsData(isSelection);
	QStringList elements;
	if (m_hasLineCount) {
		auto block_count = Utility::greaterOrEqual(counts_info.blockCount, 1);
		elements << QString::number(block_count) + " lines";
	}
	if (m_hasWordCount) {
		auto words = counts_info.text.split(
			QRegularExpression(LEADING_WHITESPACE), Qt::SkipEmptyParts);
		elements << QString::number(words.count()) + " words";
	}
	auto character_count = counts_info.text.count();
	if (m_hasCharCount)
		elements << QString::number(character_count) + " chars";
	m_counts->setText(elements.join(", "));
	emit toggleAutoCount(!(character_count >= 20000));
}

void Meter::updatePositions()
{
	if (!hasEitherPosition()) return;
	auto positions_info = emit getPositionsData();
	QStringList elements;
	if (m_hasLinePosition)
		elements << "ln " + QString::number(positions_info.cursorBlockNumber + 1);
	if (m_hasColumnPosition)
		elements << "col " + QString::number(positions_info.cursorPositionInBlock + 1);
	m_positions->setText(elements.join(", "));
}

bool Meter::toggleVisibility(QLabel* label, bool hasAnything)
{
	label->setVisible(hasAnything);
	emit separatorVisibilityCheck();
	return hasAnything;
}
