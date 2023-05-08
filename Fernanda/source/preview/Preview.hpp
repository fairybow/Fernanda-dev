#pragma once

#include "../common/Layout.hpp"
#include "../common/Widget.hpp"
#include "WebEngineView.hpp"

class Preview : public Widget<>
{
public:
	inline Preview(const char* name, QWidget* parent = nullptr)
		: Widget(name, parent)
	{
		Layout::box( m_view, this);
	}

private:
	WebDocument m_content;
	WebEngineView* m_view = new WebEngineView(m_content, this);

	inline QString url() { return QString("qrc:/Test.html"); }
};
