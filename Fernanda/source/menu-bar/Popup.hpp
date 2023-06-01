#pragma once

#include "../common/HtmlString.hpp"
#include "../common/Io.hpp"
#include "../common/Layout.hpp"
#include "../common/Path.hpp"
#include "../common/StringTools.hpp"
#include "../Version.hpp"
#include "PopupText.hpp"
#include "VersionChecker.hpp"

#include <QApplication>
#include <QDialog>
#include <QMessageBox>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QScrollArea>
#include <QVector>

#include <filesystem>

namespace Popup
{
	using StdFsPath = std::filesystem::path;

	namespace
	{
		inline void standardize(QMessageBox& box, const QString& text, bool hasOk = true, bool hasIcon = false, const QString& title = "Fernanda")
		{
			box.setWindowTitle(title);
			box.setText(text);
			if (hasIcon)
				box.setIconPixmap(QPixmap(":/menu-bar/Fernanda_64.png"));
			if (hasOk)
				box.setDefaultButton(box.addButton(QMessageBox::Ok));
		}
	}

	inline void checkVersion(QWidget* parent)
	{
		QMessageBox box(parent);
		standardize(box, VersionChecker::check(VER_COMPANYNAME_STR, VER_PRODUCTNAME_STR, VER_FILEVERSION_STR, parent), true, true);
		box.exec();
	}

	inline void showLicenses(QWidget* parent)
	{
		QDialog dialog(parent);
		auto scroll_area = new QScrollArea;
		scroll_area->setWidgetResizable(true);

		auto licenses = QVector<StdFsPath>{
			":/LICENSE",
			":/Fernanda/external/material-icons/LICENSE",
			":/Fernanda/external/mononoki/LICENSE",
			":/Fernanda/external/solarized/LICENSE",
			":/Fernanda/external/qt/LICENSE"
		};

		auto container = new QWidget;
		auto container_layout = Layout::box(Layout::Line::Vertically, nullptr, container, { 66, 0, 66, 0 });
		scroll_area->setWidget(container);

		for (auto& license : licenses) {
			auto text_display = new QPlainTextEdit;
			text_display->setReadOnly(true);
			text_display->setMinimumHeight(400);
			text_display->setPlainText(Io::readFile(license));
			auto label_text = HtmlString::heading(Path::qStringParentName(license), 3);
			container_layout->addWidget(Layout::container(text_display, nullptr, label_text));
			container_layout->addSpacing(66);
		}

		Layout::box(Layout::Line::Vertically, scroll_area, &dialog);
		dialog.setMinimumSize(800, 600);
		dialog.exec();
	}

	inline void about(QWidget* parent)
	{
		QMessageBox box(parent);
		standardize(box, PopupText::about(), true, true);
		auto update_text = StringTools::pad(3, "Check for updates").toLocal8Bit();
		auto update = box.addButton(QObject::tr(update_text), QMessageBox::AcceptRole);
		auto licenses = box.addButton(QObject::tr("Licenses"), QMessageBox::AcceptRole);
		auto qt = box.addButton(QObject::tr("About Qt"), QMessageBox::AcceptRole);
		QObject::connect(qt, &QPushButton::clicked, parent, QApplication::aboutQt);
		box.exec();
		if (box.clickedButton() == update)
			checkVersion(parent);
		else if (box.clickedButton() == licenses)
			showLicenses(parent);
	}
}
