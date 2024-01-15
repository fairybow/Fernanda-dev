#include "../../common/Layout.hpp"
#include "FontSelector.h"

#include <QList>
#include <QRandomGenerator>

#include <array>

FontSelector::FontSelector(const QFont& initialFont, QWidget* parent)
	: QWidget(parent), m_currentFont(initialFont)
{
	setup();
}

void FontSelector::setup()
{
	setupFontsBox();
	setupSizesBox();
	setupCheckBoxes();
	setupSampleText();
	sizing();

	syncFamiliesBox();
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
	auto sizes = {
		8, 9, 10, 11, 12, 14, 16, 18,
		20, 22, 24, 26, 28, 36, 48, 72
	};

	for (auto& size : sizes)
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
	m_sampleText->setText(sampleText());

	auto slot = [&](const QFont&) {
		QFont sample_text_font = m_currentFont;
		sample_text_font.setPointSizeF(22);
		m_sampleText->setFont(sample_text_font);
		};

	connect(this, &FontSelector::currentFontChanged, m_sampleText, slot);
}

void FontSelector::sizing()
{
	setContentsMargins(5, 5, 5, 5);

	QWidgetList boxes = { m_fontsBox, m_sizesBox, m_boldCheck, m_italicCheck };
	auto top_layout = Layout::box(Box::Horizontal, boxes);
	top_layout->setStretch(0, 1);
	top_layout->setStretch(1, 0);
	top_layout->setStretch(2, 0);
	top_layout->setStretch(3, 0);
	top_layout->setSpacing(5);

	Layout::box(Box::Vertical, this, QObjectList{ top_layout, m_sampleText });
}

void FontSelector::syncFamiliesBox()
{
	auto last_index = m_fontsBox->currentIndex();
	auto text = m_currentFont.family();
	m_fontsBox->setCurrentText(text);

	if (last_index == m_fontsBox->currentIndex())
		onFontsBoxCurrentTextChanged(text);
}

QString FontSelector::sampleText()
{
	// Resize on font change?

	static constexpr const char* pangrams[] = {
		"The quick brown fox jumps over the lazy dog.",
		"Waltz, bad nymph, for quick jigs vex.",
		"Sphinx of black quartz, judge my vow.",
		"How vexingly quick daft zebras jump!",
		"Pack my box with five dozen liquor jugs."
	};

	auto highest_pangram = std::size(pangrams);
	auto index = QRandomGenerator::global()->bounded(highest_pangram);
	auto random_line = pangrams[index];

	return QString::fromUtf8(random_line);
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
