#pragma once

#include "../common/Io.hpp"
#include "DocumentCache.hpp"

#include <QString>
#include <QUuid>

//#include <algorithm>
#include <filesystem>
//#include <map>

class Document
{
	using StdFsPath = std::filesystem::path;

public:
	Document(StdFsPath tempFolder, StdFsPath backupFolder, QWidget* parent = nullptr)
		: m_tempFolder(tempFolder), m_backupFolder(backupFolder), m_cache(100) {}

	const QString open(StdFsPath path)
	{
		// check cache first
		// clean text
		





		// pull from temp or cache, etc.
		//
		return Io::readFile(path);
	}

private:
	DocumentCache m_cache;
	const StdFsPath m_tempFolder;
	const StdFsPath m_backupFolder;

	//std::map<QUuid, StdFsPath> m_openFiles;
};

/*QUuid id;

		auto it = std::find_if(m_openFiles.begin(), m_openFiles.end(),
			[&](const auto& open_file) { return open_file.second == path; });
		if (it != m_openFiles.end()) {
			id = it->first;
		}
		else {
			id = QUuid::createUuid();
			m_openFiles[id] = path;
		}

		auto temp_path = m_tempFolder / Path::toStdFs(QString(id.toString() + ".txt"));*/
