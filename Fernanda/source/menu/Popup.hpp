#pragma once

#include "PopupText.hpp"
#include "VersionChecker.hpp"

#include <QApplication>
#include <QMessageBox>
#include <QPushButton>

class Popup : public QObject
{
	Q_OBJECT

public:
	static inline bool about(QWidget* parent)
	{
		auto perform_update = false;
		QMessageBox box(parent);
		standardize(box, PopupText::about(), true, true);
		auto update_text = PopupText::Buttons::update().toLocal8Bit();
		auto update = box.addButton(tr(update_text), QMessageBox::AcceptRole);
		auto qt = box.addButton(tr("About Qt"), QMessageBox::AcceptRole);
		connect(qt, &QPushButton::clicked, parent, QApplication::aboutQt);
		box.exec();
		if (box.clickedButton() == update)
			perform_update = true;
		return perform_update;
	}

	static inline void version(QWidget* parent)
	{
		QMessageBox box(parent);
		standardize(box, VersionChecker::check(VER_COMPANYNAME_STR, VER_PRODUCTNAME_STR, VER_FILEVERSION_STR, parent), true, true);
		box.exec();
	}

private:
	static inline void standardize(QMessageBox& box, const QString& text, bool hasOk = true, bool hasIcon = false, const QString& title = "Fernanda")
	{
		box.setWindowTitle(title);
		box.setText(text);
		if (hasIcon)
			box.setIconPixmap(QPixmap(":/menu/Fernanda_64.png"));
		if (hasOk)
			box.setDefaultButton(box.addButton(QMessageBox::Ok));
	}
};
