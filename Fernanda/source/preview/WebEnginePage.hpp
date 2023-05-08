#pragma once

#include <QDesktopServices>
#include <QWebEnginePage>

class WebEnginePage : public QWebEnginePage
{
public:
	using QWebEnginePage::QWebEnginePage;

protected:
	inline virtual bool acceptNavigationRequest(const QUrl& url, NavigationType, bool) override
	{
		if (url.scheme() == QStringLiteral("qrc"))
			return true;
		QDesktopServices::openUrl(url);
		return false;
	}
};
