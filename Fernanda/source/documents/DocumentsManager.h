#pragma once

#include "../common/Document.hpp"
#include "DocumentsCache.h"
#include "IdBank.hpp"

#include <QMainWindow>

#include <filesystem>

namespace StdFs = std::filesystem;

class DocumentsManager : public QObject
{
	Q_OBJECT

public:
	using StdFsPath = StdFs::path;

	struct Folders {
		StdFsPath user;
		StdFsPath temp;
		StdFsPath backup;
	};

	DocumentsManager(
		const Folders& folders,
		QMainWindow* mainWindow,
		QWidget* parent = nullptr,
		int cacheMaxCost = 100)
	{
		//
	}

	//

signals:
	//

private:
	//auto& cache = DoxCache::instance()
	static IdBank s_idBank;
};

using DoxManager = DocumentsManager;
