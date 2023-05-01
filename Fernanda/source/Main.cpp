#include "common/Path.hpp"
#include "LaunchCop.hpp"
#include "MainWindow.h"

#include <QApplication>
#include <QFont>

#include <filesystem>

void setFont(QApplication& application);

int main(int argc, char* argv[])
{
	auto window_name = "MainWindow";
	LaunchCop launch_cop("Fernanda", window_name);
	if (launch_cop.exists())
		return 0;

	QApplication::setHighDpiScaleFactorRoundingPolicy(
		Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
	QApplication::setDesktopSettingsAware(true);
	QApplication fernanda(argc, argv);

	std::filesystem::path open_file;
	for (auto& arg : fernanda.arguments())
		if (arg.endsWith(".story"))
			open_file = Path::toStdFs(arg);

	MainWindow main_window(window_name, fernanda.arguments().contains("-dev"), open_file);
	setFont(fernanda);
	main_window.show();
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
