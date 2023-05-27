#pragma once

#include "../common/Icon.hpp"
#include "TabButton.hpp"

class CloseTab : public TabButton
{
	Q_OBJECT

public:
	CloseTab(QWidget* parent = nullptr)
		: TabButton("TabButton", Icon::Ui::Close, parent, Icon::Ui::Ellipse) {}

	bool edited() const { return flagged(); }
	void setEdited(bool edited) { setFlagged(edited); }
};
