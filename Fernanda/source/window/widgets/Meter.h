#pragma once

#include "../../common/UiButton.hpp"

#include <QDebug>
#include <QLabel>
#include <QPlainTextEdit>
#include <QWidget>

class Meter : public QWidget
{
	Q_OBJECT

public:
	Meter(QWidget* parent = nullptr, int autoCountCharLimit = 15000);
	~Meter() { qDebug() << __FUNCTION__; }

	QPlainTextEdit* currentEditor() const;
	void setCurrentEditor(QPlainTextEdit* editor);
	int autoCountCharLimit() const;
	void setAutoCountCharLimit(int limit);
	bool hasPositionLabels() const;
	void setHasPositionLabels(bool has);
	bool hasCountLabels() const;
	void setHasCountLabels(bool has);
	bool useShortLabels() const;
	void setUseShortLabels(bool use);
	bool hasLinePosition() const;
	void setHasLinePosition(bool has);
	bool hasColumnPosition() const;
	void setHasColumnPosition(bool has);
	bool hasLineCount() const;
	void setHasLineCount(bool has);
	bool hasWordCount() const;
	void setHasWordCount(bool has);
	bool hasCharCount() const;
	void setHasCharCount(bool has);

	void run();
	void reset();

private:
	enum class Force {
		No,
		Yes
	};

	enum class Label {
		CharCount,
		ColPos,
		LineCount,
		LinePos,
		WordCount
	};

	QPlainTextEdit* m_currentEditor = nullptr;
	QLabel* m_positions = new QLabel(this);
	QLabel* m_counts = new QLabel(this);
	QLabel* m_separator = new QLabel(this);
	UiButton* m_refreshCounts = new UiButton(UiButton::Ui::Refresh, this);

	int m_autoCountCharLimit;
	bool m_hasPositionLabels = true;
	bool m_hasCountLabels = true;
	bool m_useShortLabels = true;
	bool m_hasLinePosition = true;
	bool m_hasColumnPosition = true;
	bool m_hasLineCount = true;
	bool m_hasWordCount = true;
	bool m_hasCharCount = true;
	bool m_autoCount = true;

	bool hasAnyPosition() const;
	bool hasAnyCount() const;

	void setup();
	void connectEditor();
	void disconnectEditor();

	void updatePositions();
	void updateCounts(Force force = Force::No);
	QString positions();
	QString counts();
	QString label(Label type) const;
	void maybeShowSubWidget(QLabel* label, bool show);
	void maybeToggleAutoCount(int characters);
	void maybeToggleRefreshCounts();
	int selectedLineCount() const;
	void hideAll();

private slots:
	void onRefreshCountsClicked();
	void onSelectionChanged();
};
