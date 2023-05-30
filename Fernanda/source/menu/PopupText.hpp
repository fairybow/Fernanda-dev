#pragma once

#include "../common/HtmlString.hpp"
#include "../Version.hpp"

namespace PopupText
{
	namespace
	{
		struct {
			int level = 3;
		} details;

		inline const QString repo()
		{
			return HtmlString::link("https://github.com/fairybow/Fernanda");
		}

		inline const QString multiSpaces(int spaces = 3)
		{
			return HtmlString::multiply(" ", spaces);
		}

		inline const QString pad(QString text, int spaces = 3) // replace with StringTools version
		{
			QString padding = multiSpaces(spaces);
			return padding + text + padding;
		}

		//inline const QString pad(const char* text, int spaces = 3) { return pad(QString(text), spaces); }
	}

	namespace Buttons
	{
		inline const QString update()
		{
			return pad("Check for updates");
		}
	}

	inline const QString about()
	{
		return
		{
			HtmlString::heading("About Fernanda", details.level)
			% "Fernanda is a plain text editor for drafting long-form fiction. (At least, that's the plan.)"
			% "It's a personal project and a work-in-progress."
			% HtmlString::heading("Version", details.level)
			% VER_FILEVERSION_STR
			% "See " + repo() + " for more information."
		};
	}
}
