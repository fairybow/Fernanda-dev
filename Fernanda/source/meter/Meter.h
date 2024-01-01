#pragma once

#include "../common/UiButton.hpp"

#include <QDebug>
#include <QLabel>
#include <QPlainTextEdit>

class Meter : public QWidget
{
	Q_OBJECT

public:
	Meter(QWidget* parent = nullptr);
	~Meter() { qDebug() << __FUNCTION__; }

	QPlainTextEdit* currentEditor() const;
	void setCurrentEditor(QPlainTextEdit* editor);

	void run();
	void clear();

private:
	enum class Force {
		No,
		Yes
	};

	QPlainTextEdit* m_currentEditor = nullptr;
	QLabel* m_positions = new QLabel(this);
	QLabel* m_counts = new QLabel(this);
	QLabel* m_separator = new QLabel(this);
	UiButton* m_refresh = new UiButton(UiButton::Ui::Refresh, this);

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
	void maybeShowLabel(QLabel* label, bool show);
	void maybeShowSeparator();
	void maybeShowRefresh();
	int selectedLineCount() const;
	void hideAll();

private slots:
	void onRefreshClicked();
	void onSelectionChanged();
};
