#pragma once

#include "../common/Io.hpp"
#include "../common/Layout.hpp"
#include "../common/Widget.hpp"
#include "TrueEditor.h"

#include <QGraphicsBlurEffect>
#include <QLabel>
#include <QPushButton>
#include <QScrollBar>
#include <QVector>

class Editor : public Widget<>
{
	Q_OBJECT

public:
	inline Editor(const char* name, QWidget* parent = nullptr)
		: Widget(name, parent)
	{
		m_trueEditor->setObjectName(name);
		m_lineNumberArea->setObjectName(name + QString("-line-number-area"));
		m_shadow->setObjectName(name + QString("-shadow"));
		m_overlay->setObjectName(name + QString("-overlay"));
		m_underlay->setObjectName(name + QString("-underlay"));
		m_trueEditor->horizontalScrollBar()->setObjectName("HScrollBar"); // rename in qss
		m_trueEditor->verticalScrollBar()->setObjectName("VScrollBar"); // rename in qss
		m_scrollUp->setObjectName("ScrollUp"); // rename in qss
		m_scrollPrevious->setObjectName("ScrollPrevious"); // rename in qss
		m_scrollNext->setObjectName("ScrollNext"); // rename in qss
		m_scrollDown->setObjectName("ScrollDown"); // rename in qss

		m_trueEditor->setReadOnly(true);
		m_trueEditor->setLineNumberArea(m_lineNumberArea);
		m_trueEditor->viewport()->setCursor(Qt::ArrowCursor);

		m_shadow->setStyleSheet(Io::readFile(":/editor/Shadow.qss")); // blur

		m_shadow->setAttribute(Qt::WA_TransparentForMouseEvents);
		m_overlay->setAttribute(Qt::WA_TransparentForMouseEvents);

		auto effect = new QGraphicsBlurEffect(this);
		effect->setBlurHints(QGraphicsBlurEffect::QualityHint);
		effect->setBlurRadius(15);
		m_shadow->setGraphicsEffect(effect);

		buildScrollBar();

		Layout::stack({ m_shadow, m_overlay, m_trueEditor, m_underlay }, this);
	}

	// funnel to and from QPTE
	void setReadOnly(bool readOnly) { m_trueEditor->setReadOnly(readOnly); }
	bool isReadOnly() const { return m_trueEditor->isReadOnly(); }
	// add when needed

private:
	TrueEditor* m_trueEditor = new TrueEditor(this);
	LineNumberArea* m_lineNumberArea = new LineNumberArea(m_trueEditor);
	QLabel* m_shadow = new QLabel(this);
	QLabel* m_overlay = new QLabel(this);
	QLabel* m_underlay = new QLabel(this);
	QPushButton* m_scrollPrevious = new QPushButton;
	QPushButton* m_scrollUp = new QPushButton;
	QPushButton* m_scrollDown = new QPushButton;
	QPushButton* m_scrollNext = new QPushButton;

	void buildScrollBar()
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
	}
};
