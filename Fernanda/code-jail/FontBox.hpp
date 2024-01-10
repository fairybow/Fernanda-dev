#pragma once

#include "LiveFontDialog.hpp"

#include <QDebug>
#include <QFont>
#include <QGridLayout>
#include <QGroupBox>
#include <QMargins>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QString>

class FontBox : public QGroupBox
{
	Q_OBJECT

public:
	FontBox(const QFont& initial, const QString& title = QString(), QWidget* parent = nullptr)
		: QGroupBox(title, parent), m_fontDialog(new LiveFontDialog(initial, this))
	{
		setup();
	}

	~FontBox()
	{
		qDebug() << __FUNCTION__;

		delete m_mdiArea;
	}

private:
	LiveFontDialog* m_fontDialog;
	QMdiArea* m_mdiArea = new QMdiArea;

	void setup()
	{
		auto mdi_sub_window = m_mdiArea->addSubWindow(m_fontDialog);
		mdi_sub_window->setWindowFlags(Qt::FramelessWindowHint);
		mdi_sub_window->showMaximized();

		auto grid = new QGridLayout;
		grid->setContentsMargins(QMargins(5, 5, 5, 5));
		grid->setSpacing(5);
		grid->addWidget(m_mdiArea);

		setLayout(grid);

		connect(m_fontDialog, &LiveFontDialog::currentFontChanged, this, [&](const QFont& font) {
			emit currentFontChanged(font);
			});
	}

signals:
	void currentFontChanged(const QFont& font);
};
