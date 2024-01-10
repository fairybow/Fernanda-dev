#pragma once

#include <QDebug>
#include <QFont>
#include <QFontDialog>

class LiveFontDialog : public QFontDialog
{
	Q_OBJECT

public:
	LiveFontDialog(const QFont& initial, QWidget* parent = nullptr)
		: QFontDialog(initial, parent)
	{
		setOptions(LiveFontDialog::NoButtons);
	}

	~LiveFontDialog()
	{
		qDebug() << __FUNCTION__;
	}

	void accept() override {}
};
