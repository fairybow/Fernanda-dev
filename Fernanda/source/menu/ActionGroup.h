#pragma once

#include "../common/Path.hpp"

#include <QActionGroup>
#include <QDirIterator>
#include <QString>
#include <QVector>

#include <algorithm>
#include <functional>

class ActionGroup : public QActionGroup
{
	using StdFsPath = std::filesystem::path;
	using StdFsPathList = QVector<StdFsPath>;

	Q_OBJECT

public:
	using QActionGroup::QActionGroup;

	struct Bespoke {
		QVariant data;
		QString label = QString();
	};

	using BespokeList = QVector<Bespoke>;

	static Bespoke bespoke(QVariant data, QString label = QString());
	static ActionGroup* fromQrc(const QStringList& qrcPaths, QStringList extensions,
		StdFsPathList systemPaths = {}, QObject* parent = nullptr, std::function<void()> slot = nullptr);
	static ActionGroup* fromQrc(const QString& qrcPath, QString extension,
		StdFsPath systemPath = StdFsPath(), QObject* parent = nullptr, std::function<void()> slot = nullptr);
	static ActionGroup* fromQrc(const QStringList& qrcPaths, QStringList extensions,
		StdFsPath systemPath = StdFsPath(), QObject* parent = nullptr, std::function<void()> slot = nullptr);
	static ActionGroup* fromBespoke(BespokeList entries, QObject* parent = nullptr,
		std::function<void()> slot = nullptr);

private:
	static void addActionToGroup(ActionGroup* actionGroup, const QString& label,
		const QVariant& data, QObject* parent, std::function<void()> slot);
	static void checkExtensions(QStringList& extensions);
	static void alphabetize(ActionGroup* actionGroup);
	static QStringList gather(const QStringList& qrcPaths, const QStringList& extensions);
};
