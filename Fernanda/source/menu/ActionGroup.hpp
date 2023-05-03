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
		QString label = QString();
	};

	using BespokeList = QVector<Bespoke>;

	static inline ActionGroup* fromQrc(const QStringList& qrcPaths, QStringList extensions,
		StdFsPathList systemPaths = {}, QObject* parent = nullptr, std::function<void()> slot = nullptr)
	{
		auto group = new ActionGroup(parent);
		checkExtensions(extensions);
		auto entries = gather(qrcPaths, extensions);
		for (auto& entry : entries) {
			auto label = Path::qStringName(entry);
			addActionToGroup(group, label, entry, parent, slot);
		}
		alphabetize(group);
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

	static inline ActionGroup* bespoke(BespokeList entries, QObject* parent = nullptr,
		std::function<void()> slot = nullptr)
	{
		auto group = new ActionGroup(parent);
		for (auto& data_pair : entries) {
			auto& label = data_pair.label;
			if (label.isEmpty())
				label = data_pair.data.toString();
			addActionToGroup(group, label, data_pair.data, parent, slot);
		}
		alphabetize(group);
		group->setExclusive(true);
		return group;
	}

private:
	static inline void addActionToGroup(ActionGroup* actionGroup, const QString& label,
		const QVariant& data, QObject* parent, std::function<void()> slot)
	{
		auto action = new QAction(tr(label.toUtf8()), actionGroup);
		action->setData(data);
		action->setCheckable(true);
		connect(action, &QAction::toggled, parent, slot);
	}

	static inline void checkExtensions(QStringList& extensions)
	{
		for (auto& extension : extensions)
			if (!extension.startsWith("*"))
				extension = "*" + extension;
	}

	static inline void alphabetize(ActionGroup* actionGroup)
	{
		QVector<QAction*> sorted_actions = actionGroup->actions();
		std::sort(sorted_actions.begin(), sorted_actions.end(), [](auto& lhs, auto& rhs) {
			return lhs->text() < rhs->text();
			});
		for (auto& action : actionGroup->actions())
			actionGroup->removeAction(action);
		for (auto& action : sorted_actions)
			actionGroup->addAction(action);
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
