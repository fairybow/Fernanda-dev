#pragma once

#include "../common/Icon.hpp"
#include "TabButton.hpp"

#include <QUuid>

class CloseTab : public TabButton
{
	Q_OBJECT

public:
	CloseTab(QUuid id, QWidget* parent = nullptr)
		: TabButton("TabButton", Icon::Ui::Close, parent, Icon::Ui::Ellipse),
		m_id(id)
	{
		connect(this, &CloseTab::clicked, this, [&] {
			emit askClose(m_id);
			});
	}

	bool edited() const { return flagged(); }
	void setEdited(bool edited) { setFlagged(edited); }

signals:
	void askClose(QUuid id);

private:
	QUuid m_id;
};
