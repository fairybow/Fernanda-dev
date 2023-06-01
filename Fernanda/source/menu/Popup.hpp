#pragma once

#include "../common/Layout.hpp"
#include "../common/StringTools.hpp"
#include "../Version.hpp"
#include "PopupText.hpp"
#include "VersionChecker.hpp"

#include <QApplication>
#include <QDialog>
#include <QMessageBox>
#include <QPushButton>
//
#include <QScrollArea>
#include <QPlainTextEdit>
#include <QPushButton>

namespace Popup
{
	namespace
	{
		inline void standardize(QMessageBox& box, const QString& text, bool hasOk = true, bool hasIcon = false, const QString& title = "Fernanda")
		{
			box.setWindowTitle(title);
			box.setText(text);
			if (hasIcon)
				box.setIconPixmap(QPixmap(":/menu/Fernanda_64.png"));
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
		// for each license in qrc: editor and label
		auto test_editor_1 = new QPlainTextEdit;
		auto test_editor_2 = new QPlainTextEdit;
		auto test_editor_3 = new QPlainTextEdit;
		auto scroll_area = new QScrollArea;
		auto container = Layout::container({ test_editor_1, test_editor_2, test_editor_3 });
		scroll_area->setWidgetResizable(true);
		scroll_area->setWidget(container);
		QPushButton* ok_button = new QPushButton("OK", &dialog);
		QObject::connect(ok_button, &QPushButton::clicked, &dialog, &QDialog::accept);
		auto layout = Layout::box(Layout::Line::Vertically, { scroll_area, ok_button });
		dialog.setLayout(layout);
		dialog.exec();
	}

	inline void about(QWidget* parent)
	{
		QMessageBox box(parent);
		standardize(box, PopupText::about(), true, true);
		auto update_text = StringTools::pad(3, "Check for updates").toLocal8Bit();
		auto update = box.addButton(QObject::tr(update_text), QMessageBox::AcceptRole);
		auto licenses = box.addButton(QObject::tr("Licenses"), QMessageBox::AcceptRole);
		//auto qt = box.addButton(QObject::tr("About Qt"), QMessageBox::AcceptRole);
		//QObject::connect(qt, &QPushButton::clicked, parent, QApplication::aboutQt);
		box.exec();
		if (box.clickedButton() == update)
			checkVersion(parent);
		else if (box.clickedButton() == licenses)
			showLicenses(parent);
	}
}
