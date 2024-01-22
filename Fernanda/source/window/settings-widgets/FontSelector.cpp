#include "../../common/Layout.hpp"
#include "../../common/StringTools.hpp"
#include "FontSelector.h"

#include <QFontMetrics>
#include <QList>

constexpr int SIZES[] = { 144, 72, 48, 36, 28, 26, 24, 22, 20, 18, 16, 14, 12, 11, 10, 9, 8, 6 };

FontSelector::FontSelector(const QFont& initialFont, QWidget* parent)
	: QWidget(parent), m_currentFont(initialFont)
{
	setup();
}

void FontSelector::showEvent(QShowEvent* event)
{
	QWidget::showEvent(event);

	resizeSampleText();
}

void FontSelector::setup()
{
	setupFontsBox();
	setupSizesBox();
	setupCheckBoxes();
	setupSampleText();
	sizing();

	syncFontsBox();
}

void FontSelector::setupFontsBox()
{
	auto fonts = m_database.families();
	m_fontsBox->addItems(fonts);
	auto current_family = m_currentFont.family();
	m_fontsBox->setCurrentText(current_family);

	connect(m_fontsBox, &QComboBox::currentTextChanged, this, &FontSelector::onFontsBoxCurrentTextChanged);
}

void FontSelector::setupSizesBox()
{
	for (auto& size : SIZES)
		m_sizesBox->addItem(QString::number(size));

	connect(m_sizesBox, &QComboBox::currentTextChanged, this, &FontSelector::onSizesBoxCurrentTextChanged);
}

void FontSelector::setupCheckBoxes()
{
	m_boldCheck->setTristate(false);
	m_italicCheck->setTristate(false);

	m_boldCheck->setChecked(m_currentFont.bold());
	m_italicCheck->setChecked(m_currentFont.italic());

	connect(m_boldCheck, &QCheckBox::stateChanged, this, [&](int state) {
		m_currentFont.setBold(state);
		emit currentFontChanged(m_currentFont);
		});

	connect(m_italicCheck, &QCheckBox::stateChanged, this, [&](int state) {
		m_currentFont.setItalic(state);
		emit currentFontChanged(m_currentFont);
		});
}

void FontSelector::setupSampleText()
{
	m_sampleText->setAlignment(Qt::AlignCenter);
	m_sampleText->setWordWrap(true);
	m_sampleText->setText(StringTools::pangram());

	connect(this, &FontSelector::currentFontChanged, this, [&](const QFont& font) { resizeSampleText(); });
}

void FontSelector::sizing()
{
	auto space = 5;
	setContentsMargins(space, space, space, space);

	QWidgetList boxes = { m_fontsBox, m_sizesBox, m_boldCheck, m_italicCheck };
	auto top_layout = Layout::box(Box::Horizontal, boxes);
	top_layout->setStretch(0, 1);
	top_layout->setStretch(1, 0);
	top_layout->setStretch(2, 0);
	top_layout->setStretch(3, 0);
	top_layout->setSpacing(8);

	Layout::box(Box::Vertical, this, QObjectList{ top_layout, m_sampleText });
}

void FontSelector::syncFontsBox()
{
	auto last_index = m_fontsBox->currentIndex();
	auto text = m_currentFont.family();
	m_fontsBox->setCurrentText(text);

	if (last_index == m_fontsBox->currentIndex())
		onFontsBoxCurrentTextChanged(text);
}

void FontSelector::resizeSampleText()
{
	QFont sample_text_font = m_currentFont;
	auto max_point_size = 48;
	sample_text_font.setPointSizeF(max_point_size);

	QFontMetrics metrics(sample_text_font);

	while (max_point_size > 0
		&& (metrics.horizontalAdvance(m_sampleText->text()) > m_sampleText->width()
			|| metrics.height() > m_sampleText->height())) {
		max_point_size--;
		sample_text_font.setPointSizeF(max_point_size);

		metrics = QFontMetrics(sample_text_font);
	}

	m_sampleText->setFont(sample_text_font);
}

void FontSelector::onFontsBoxCurrentTextChanged(const QString& text)
{
	m_currentFont.setFamily(text);
	m_sizesBox->setCurrentText(QString::number(m_currentFont.pointSize()));

	emit currentFontChanged(m_currentFont);
}

void FontSelector::onSizesBoxCurrentTextChanged(const QString& text)
{
	m_currentFont.setPointSize(text.toInt());

	emit currentFontChanged(m_currentFont);
}
