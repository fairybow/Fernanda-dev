#pragma once

#include "../common/Fx.hpp"
#include "../common/Io.hpp"
#include "../common/Layout.hpp"
#include "../common/Widget.hpp"
#include "TrueEditor.h"

#include <QFont>
#include <QLabel>
#include <QPushButton>
#include <QVector>

class Editor : public Widget<>
{
	Q_OBJECT

public:
	Editor(const char* name, const QFont& defaultFont = QFont(), QWidget* parent = nullptr);

	inline QFont defaulFont() const { return m_defaultFont; };
	inline void setFont(const QFont& font) { m_trueEditor->setFont(font); };
	inline void setReadOnly(bool readOnly) { m_trueEditor->setReadOnly(readOnly); }
	inline bool isReadOnly() const { return m_trueEditor->isReadOnly(); }

private:
	const QFont m_defaultFont;
	TrueEditor* m_trueEditor = new TrueEditor(this);
	LineNumberArea* m_lineNumberArea = new LineNumberArea(m_trueEditor);
	QLabel* m_shadow = new QLabel(this);
	QLabel* m_overlay = new QLabel(this);
	QLabel* m_underlay = new QLabel(this);
	QPushButton* m_scrollPrevious = new QPushButton;
	QPushButton* m_scrollUp = new QPushButton;
	QPushButton* m_scrollDown = new QPushButton;
	QPushButton* m_scrollNext = new QPushButton;

	void nameObjects(const char* name);
	void setupTrueEditor();
	void setupShadow();
	void buildScrollBar();
	void connections();
	void scrollButtonEnabler();
};
