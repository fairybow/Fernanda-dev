#pragma once

#include "../common/Layout.hpp"
#include "WebEngineView.hpp"

class Preview : public QWidget
{
public:
	inline Preview(const char* name, QWidget* parent = nullptr)
		: QWidget(parent)
	{
		setObjectName(name);
		setContentsMargins(0, 0, 0, 0);
		Layout::set(this, m_view);
	}

private:
	WebDocument m_content;
	WebEngineView* m_view = new WebEngineView(m_content, this);

	inline QString url() { return QString("qrc:/Test.html"); }
};
