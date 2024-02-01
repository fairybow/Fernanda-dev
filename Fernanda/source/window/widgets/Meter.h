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
	QString linePositionLabel() const;
	void setLinePositionLabel(const QString& text);
	QString columnPositionLabel() const;
	void setColumnPositionLabel(const QString& text);
	QString lineCountLabel() const;
	void setLineCountLabel(const QString& text);
	QString wordCountLabel() const;
	void setWordCountLabel(const QString& text);
	QString charCountLabel() const;
	void setCharCountLabel(const QString& text);

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

	enum class Run {
		No,
		Yes
	};

	QPlainTextEdit* m_currentEditor = nullptr;
	QLabel* m_positions = new QLabel(this);
	QLabel* m_counts = new QLabel(this);
	QLabel* m_separator = new QLabel(this);
	UiButton* m_refreshCounts = new UiButton(UiButton::Ui::Refresh, this);
	QString m_linePositionLabel;
	QString m_columnPositionLabel;
	QString m_lineCountLabel;
	QString m_wordCountLabel;
	QString m_charCountLabel;
	int m_autoCountCharLimit;
	bool m_hasPositionLabels = true;
	bool m_hasCountLabels = true;
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
	QString labelText(bool has, const QString& customLabel, const QString& defaultLabel) const;
	void maybeShowSubWidget(QLabel* label, bool show);
	void maybeToggleAutoCount(int characters);
	void maybeToggleRefreshCounts();
	int selectedLineCount() const;
	void hideAll();

	template <typename T>
	void setMember(T& memberValue, const T& newValue, Run runAfterward = Run::No)
	{
		if (memberValue == newValue) return;

		memberValue = newValue;

		if (runAfterward == Run::Yes)
			run();
	}

private slots:
	void onRefreshCountsClicked();
	void onSelectionChanged();
};
