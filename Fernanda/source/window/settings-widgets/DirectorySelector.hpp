#pragma once

#include "../../common/Layout.hpp"
#include "../../common/Path.hpp"

#include <QDebug>
#include <QFileDialog>
#include <QLabel>
#include <QPushButton>
#include <QWidget>

class DirectorySelector : public QWidget
{
	Q_OBJECT

public:
	DirectorySelector(QWidget* parent = nullptr, const Path& initialPath = Path::system(Path::System::Home))
		: QWidget(parent), m_currentPath(initialPath)
	{
		setup();
	}

	~DirectorySelector()
	{
		qDebug() << __FUNCTION__;
	}

private:
	static constexpr char BUTTON_TEXT[] = "Choose...";
	static constexpr char DIALOG_TEXT[] = "Set directory";

	Path m_currentPath;
	QLabel* m_currentPathDisplay = new QLabel(m_currentPath.toQString(), this);

	void setup()
	{
		auto space = 5;
		setContentsMargins(space, space, space, space);

		auto button = new QPushButton(BUTTON_TEXT, this);

		connect(button, &QPushButton::pressed, this, [&] {

			auto directory = QFileDialog::getExistingDirectory(this, DIALOG_TEXT, m_currentPath.toQString());

			if (directory.isEmpty()) return;

			m_currentPath = directory;
			m_currentPathDisplay->setText(directory);

			emit selected(m_currentPath);

			});

		auto layout = Layout::box(Layout::Orientation::Horizontal, this, QWidgetList{ button, m_currentPathDisplay });
		layout->setStretch(0, 0);
		layout->setStretch(1, 1);
		layout->setSpacing(8);
	}

signals:
	void selected(const Path& directory);
};
