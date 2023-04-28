#pragma once

#include "../Version.hpp"
#include "HtmlString.hpp"

#include <QJsonDocument>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class VersionChecker : public QObject
{
	Q_OBJECT

public:
	static inline const QString check(QWidget* parent)
	{
		QString text = {
			HtmlString::heading("Version") %
			HtmlString::bold("Current version:") /
			VER_FILEVERSION_STR
		};

		// etc.

		return text;
	}
};
