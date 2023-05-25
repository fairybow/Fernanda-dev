#pragma once

#include "common/StringTools.hpp"

#include <QCoreApplication>
#include <QFile>
#include <QTextStream>

#ifdef Q_OS_WINDOWS

#include <Windows.h>

#endif

#include <filesystem>

namespace Logger
{
	using StdFsPath = std::filesystem::path;

	namespace
	{
		static bool isInitialized = false;
		static QFile outFile;
		static QTextStream stream(&outFile);

		inline void toConsole(const QString& message)
		{

#ifdef Q_OS_WINDOWS

			auto wstr = message.toStdWString();
			OutputDebugStringW(wstr.c_str());

#endif

			std::fprintf(stderr, "%s\n", message.toLocal8Bit().constData());
		}

		inline void clearOnFirstWrite()
		{
			outFile.close();
			outFile.open(QIODevice::WriteOnly);
			outFile.close();
			outFile.open(QIODevice::WriteOnly | QIODevice::Append);
			isInitialized = true;
		}

		inline void log(const QString& message)
		{
			stream << StringTools::time()
				<< "\n" << StringTools::clean(message)
				<< "\n" << Qt::endl;
		}

		inline void passthrough(QtMsgType type, const QMessageLogContext& context, const QString& message)
		{
			toConsole(message);
			if (!isInitialized)
				clearOnFirstWrite();
			log(message);
		}
	}

	void install(const StdFsPath& path)
	{
		outFile.setFileName(path / "Debug.log");
		outFile.open(QIODevice::WriteOnly | QIODevice::Append);
		qInstallMessageHandler(passthrough);
	}
}
