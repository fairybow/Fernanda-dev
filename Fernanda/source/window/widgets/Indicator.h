#pragma once

#include <QDebug>
#include <QMainWindow>
#include <QProgressBar>
#include <QTimer>
#include <QWidget>

class Indicator : public QWidget
{
	Q_OBJECT

public:
	Indicator(QMainWindow* parent = nullptr);
	~Indicator() { qDebug() << __FUNCTION__; }

	enum class Placement {
		Bottom,
		Top
	};

	Placement placement() const;
	void setPlacement(Placement placement);

	void green(int delay = 0);
	void pastel(int delay = 0);
	void red(int delay = 0);

private:
	enum class Scheme {
		Green,
		Pastel,
		Red
	};

	QProgressBar* m_progressBar = new QProgressBar(this);
	QTimer* m_holder = new QTimer(this);
	QTimer* m_resizer = new QTimer(this);
	Placement m_placement = Placement::Top;

	void setup();
	void setupProgressBar();
	void setupTimers();
	void start(int delay, Scheme colorScheme);
	void setBarStyle(Scheme colorScheme);
	QString styleSheet(Scheme colorScheme) const;
	int yPosition() const;

private slots:
	void onHolderTimeout();
	void onResizerTimeout();
};
