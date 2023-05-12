#pragma once

#include "../common/Io.hpp"
#include "../common/Path.hpp"

#include <QString>

class Document
{
	using StdFsPath = std::filesystem::path;

public:
	Document(QWidget* parent = nullptr) {}

	const QString open(StdFsPath path)
	{
		// handle tabs, pull from temp or cache, etc.
		return Io::readFile(path);
	}
};
