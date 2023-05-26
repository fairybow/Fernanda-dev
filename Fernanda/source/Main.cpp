#include "common/Path.hpp"
#include "common/Utility.hpp"
#include "Logger.hpp"
#include "MainWindow.h"

#include <QApplication>
#include <QFont>

#include <filesystem>

void setFont(QApplication& application);

int main(int argc, char* argv[])
{
	QApplication::setHighDpiScaleFactorRoundingPolicy(
		Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
	QApplication::setDesktopSettingsAware(true);
	QApplication fernanda(argc, argv);

	std::filesystem::path open_file;
	for (auto& arg : fernanda.arguments())
		if (arg.endsWith(".txt")) // handle projects, too
			open_file = Path::toStdFs(arg);

	MainWindow main_window("MainWindow", fernanda.arguments().contains("-dev"), open_file);
	setFont(fernanda);
	main_window.show();
	Logger::install(main_window.userData());
	Utility::ensureAppVisible(fernanda, main_window);
	return fernanda.exec();
}

void setFont(QApplication& application)
{
	auto font = application.font();
	font.setStyleStrategy(QFont::PreferAntialias);
	font.setHintingPreference(QFont::HintingPreference::PreferNoHinting);
	font.setPointSizeF(9);
	application.setFont(font);
}
