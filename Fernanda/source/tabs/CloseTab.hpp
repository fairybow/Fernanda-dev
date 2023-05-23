#pragma once

#include "../common/Svg.hpp"
#include "TabButton.hpp"

#include <QUuid>

class CloseTab : public TabButton
{
	Q_OBJECT

public:
	CloseTab(QUuid id, QWidget* parent = nullptr)
		: TabButton("TabButton", Svg::Ui::Close, parent, Svg::Ui::Ellipse, 1.0, 0.5),
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
