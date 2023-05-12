#pragma once

#include "../common/Io.hpp"

#include <QString>

#include <filesystem>

class Document
{
	using StdFsPath = std::filesystem::path;

public:
	Document(StdFsPath tempFolder, StdFsPath backupFolder, QWidget* parent = nullptr)
		: m_tempFolder(tempFolder), m_backupFolder(backupFolder) {}

	const QString open(StdFsPath path)
	{
		// handle tabs, pull from temp or cache, etc.
		return Io::readFile(path);
	}

private:
	const StdFsPath m_tempFolder;
	const StdFsPath m_backupFolder;
};
