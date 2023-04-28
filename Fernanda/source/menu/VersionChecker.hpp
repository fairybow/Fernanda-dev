#pragma once

#include "../Version.hpp"
#include "HtmlString.hpp"

#include <QEventLoop>
#include <QJsonDocument>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QProgressDialog>
#include <QVariantMap>

class VersionChecker : public QObject
{
	Q_OBJECT

public:
	static inline const QString check(const QUrl& gitHubReleasesApi, const QUrl& gitHubReleases, QWidget* parent)
	{
		QString text = {
			HtmlString::heading("Version") %
			HtmlString::bold("Current version:") /
			VER_FILEVERSION_STR
		};
		auto map = latestVersion(gitHubReleasesApi, parent);
		if (!map.isEmpty()) {
			auto latest = map["tag_name"].toString();
			if (latest == QString(VER_FILEVERSION_STR))
				text += QStringLiteral("You have the latest version.");
			else {
				QString message = {
					HtmlString::bold("New version:") /
					latest %
					QStringLiteral("You do not have the latest version.") %
					HtmlString::bold("Download:") /
					HtmlString::link(gitHubReleases)
				};
				text %= message;
			}
		}
		else {
			QString message = {
				QStringLiteral("Unable to verify version.") %
				HtmlString::bold("Check:") /
				HtmlString::link(gitHubReleases)
			};
			text %= message;
		}
		return text;
	}

private:
	static inline QVariantMap latestVersion(const QUrl& url, QWidget* parent)
	{
		QEventLoop loop;
		auto manager = new QNetworkAccessManager(parent);
		auto request = QNetworkRequest(url);
		auto reply = manager->get(request);
		connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
		QProgressDialog progress_dialog("Checking...", nullptr, 0, 0, parent);
		progress_dialog.setWindowModality(Qt::WindowModal);
		progress_dialog.show();
		loop.exec();
		progress_dialog.hide();
		QVariantMap map;
		if (reply->error() == QNetworkReply::NoError) {
			auto json_document = QJsonDocument::fromJson(reply->readAll());
			auto variant_list = json_document.toVariant().toList();
			map = variant_list[0].toMap();
		}
		reply->deleteLater();
		manager->deleteLater();
		return map;
	}
};
