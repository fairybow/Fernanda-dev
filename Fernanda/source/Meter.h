#pragma once

#include "common/Fx.hpp"
#include "common/Layout.hpp"
#include "common/RegexPatterns.hpp"
#include "common/Widget.hpp"
#include "common/StatusBarButton.hpp"

#include <QLabel>
#include <QRegularExpression>
#include <QString>

class Meter : public Widget<>
{
	Q_OBJECT

public:
	enum class Type {
		Counts,
		Positions,
		Selection
	};

	struct Counts {
		QString text;
		int blockCount;
	};
	struct Positions {
		int cursorBlockNumber;
		int cursorPositionInBlock;
	};

	Meter(const char* name, QWidget* parent = nullptr);

	void trigger(Type type, bool force = false);

signals:
	void separatorVisibilityCheck();
	void toggleAutoCount(bool checked);
	void editorFocusReturn();
	Meter::Counts getCountsData(bool isSelection);
	Meter::Positions getPositionsData();

private:
	QLabel* m_positions = new QLabel(this);
	QLabel* m_counts = new QLabel(this);
	QLabel* m_separator = new QLabel(this);
	StatusBarButton* m_refresh = new StatusBarButton("StatusBarButton", "\U0001F504", this, 0.4);

	bool m_hasAutoCount = true;
	bool m_hasLinePosition = true;
	bool m_hasColumnPosition = true;
	bool m_hasLineCount = true;
	bool m_hasWordCount = true;
	bool m_hasCharCount = true;

	void setupLabels();
	void connections();
	void updateCounts(bool isSelection = false);
	void updatePositions();
	bool toggleVisibility(QLabel* label, bool hasAnything);

	bool hasAnyCount() { return toggleVisibility(m_counts, (m_hasLineCount || m_hasWordCount || m_hasCharCount)); }
	bool hasEitherPosition() { return toggleVisibility(m_positions, (m_hasLinePosition || m_hasColumnPosition)); }
};
