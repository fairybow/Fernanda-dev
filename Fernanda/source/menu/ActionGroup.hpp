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
		const QVariant data;
		const QString label = QString();
	};

	using BespokeList = QVector<Bespoke>;

	static inline ActionGroup* fromQrc(const QStringList& qrcPaths, QStringList extensions,
		StdFsPathList systemPaths = {}, QObject* parent = nullptr, std::function<void()> slot = nullptr)
	{
		auto group = new ActionGroup(parent);
		checkExtensions(extensions);
		auto entries = gather(qrcPaths, extensions);
		abcByFileName(entries);

		/*for (auto& entry : entries) {
			auto label = Path::qStringName(entry).toUtf8();
			auto action = new QAction(tr(label), group);
			action->setData(entry);
			action->setCheckable(true);
			QObject::connect(action, &QAction::toggled, parent, slot);
		}*/

		group->setExclusive(true);
		return group;
	}

	static inline ActionGroup* fromQrc(const QString& qrcPath, QString extension,
		StdFsPath systemPath = StdFsPath(), QObject* parent = nullptr, std::function<void()> slot = nullptr)
	{
		return fromQrc(QStringList{ qrcPath }, QStringList{ extension },
			StdFsPathList{ systemPath }, parent, slot);
	}

	static inline ActionGroup* fromQrc(const QStringList& qrcPaths, QStringList extensions,
		StdFsPath systemPath = StdFsPath(), QObject* parent = nullptr, std::function<void()> slot = nullptr)
	{
		return fromQrc(qrcPaths, extensions, StdFsPathList{ systemPath }, parent, slot);
	}

	static inline ActionGroup* bespoke(BespokeList entries, QObject* parent = nullptr, std::function<void()> slot = nullptr)
	{
		auto group = new ActionGroup(parent);
		
		/*for (auto& data_pair : entries) {
			auto label = data_pair.label.toUtf8();
			auto action = new QAction(tr(label), group);
			action->setData(data_pair.data);
			action->setCheckable(true);
			QObject::connect(action, &QAction::toggled, parent, slot);
		}*/

		group->setExclusive(true);
		return group;
	}

private:
	static inline void checkExtensions(QStringList& extensions)
	{
		for (auto& extension : extensions)
			if (!extension.startsWith("*"))
				extension = "*" + extension;
	}

	static inline void abcByFileName(QStringList& paths)
	{
		std::sort(paths.begin(), paths.end(), [](auto& lhs, auto& rhs) {
			return Path::name(lhs) < Path::name(rhs);
			});
	}

	static inline QStringList gather(const QStringList& qrcPaths, const QStringList& extensions)
	{
		QStringList entries;
		for (auto& qrc_path : qrcPaths) {
			QDirIterator it(qrc_path, extensions, QDir::Files, QDirIterator::Subdirectories);
			while (it.hasNext()) {
				it.next();
				entries << it.filePath();
			}
		}
		return entries;
	}
};
