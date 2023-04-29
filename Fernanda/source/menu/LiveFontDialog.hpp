#pragma once

#include <QFontDialog>

class LiveFontDialog : public QFontDialog
{
public:
	using QFontDialog::QFontDialog; // Needed?

	void accept() override {}
};
