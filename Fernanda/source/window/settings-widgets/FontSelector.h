#pragma once

#include <QCheckBox>
#include <QComboBox>
#include <QDebug>
#include <QFont>
#include <QFontDatabase>
#include <QLabel>
#include <QShowEvent>
#include <QString>
#include <QWidget>

class FontSelector : public QWidget
{
	Q_OBJECT

public:
	FontSelector(const QFont& initialFont = QFont(), QWidget* parent = nullptr);
	~FontSelector() { qDebug() << __FUNCTION__; }

protected:
	void showEvent(QShowEvent* event) override;

private:
	QFont m_currentFont;
	QFontDatabase m_database;
	QComboBox* m_fontsBox = new QComboBox(this);
	QComboBox* m_sizesBox = new QComboBox(this);
	QCheckBox* m_boldCheck = new QCheckBox("Bold", this);
	QCheckBox* m_italicCheck = new QCheckBox("Italic", this);
	QLabel* m_sampleText = new QLabel(this);

	void setup();
	void setupFontsBox();
	void setupSizesBox();
	void setupCheckBoxes();
	void setupSampleText();
	void sizing();
	void syncFontsBox();
	void resizeSampleText();

private slots:
	void onFontsBoxCurrentTextChanged(const QString& text);
	void onSizesBoxCurrentTextChanged(const QString& text);

signals:
	void currentFontChanged(const QFont& font);
};
