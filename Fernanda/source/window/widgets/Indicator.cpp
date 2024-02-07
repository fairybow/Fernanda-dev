#include "../../common/Layout.hpp"
#include "Indicator.h"

#include <QList>
#include <QMenuBar>
#include <QStatusBar>
#include <QTimeLine>

static constexpr int DEFAULT_HEIGHT = 3;
static constexpr int FILL_TIME = 300;
static constexpr int HOLD_TIME = 1500;
static constexpr int MAX_DELAY = 3000;
static constexpr int MAX_RANGE = 100;
static constexpr int MIN_RANGE = 0;
static constexpr int RESIZE_INTERVAL = 0;

static constexpr char QSS_FORMAT[] = "\
QProgressBar::chunk \
{ \
	background: qlineargradient( \
		x1: 0, \
		y1: 0, \
		x2: 1, \
		y2: 0, \
		stop: 0 #%1, \
		stop: 0.25 #%2, \
		stop: 0.75 #%3, \
		stop: 1 #%4 \
	); \
}";

Indicator::Indicator(QMainWindow* parent)
	: QWidget(parent)
{
	setup();
}

Indicator::Placement Indicator::placement() const
{
	return m_placement;
}

void Indicator::setPlacement(Placement placement)
{
	m_placement = placement;
}

void Indicator::green(int delay)
{
	start(delay, Scheme::Green);
}

void Indicator::pastel(int delay)
{
	start(delay, Scheme::Pastel);
}

void Indicator::red(int delay)
{
	start(delay, Scheme::Red);
}

void Indicator::setup()
{
	setFixedHeight(DEFAULT_HEIGHT);
	setAttribute(Qt::WA_TransparentForMouseEvents);

	setupProgressBar();
	setupTimers();
	
	Layout::box(Layout::Orientation::Vertical, this, QWidgetList{ m_progressBar });
}

void Indicator::setupProgressBar()
{
	m_progressBar->setAttribute(Qt::WA_TransparentForMouseEvents);
	m_progressBar->setTextVisible(false);
	m_progressBar->setRange(MIN_RANGE, MAX_RANGE);
	m_progressBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	m_progressBar->hide();
}

void Indicator::setupTimers()
{
	m_holder->setSingleShot(true);
	connect(m_holder, &QTimer::timeout, this, &Indicator::onHolderTimeout);

	m_resizer->setSingleShot(false);
	m_resizer->setInterval(RESIZE_INTERVAL);
	connect(m_resizer, &QTimer::timeout, this, &Indicator::onResizerTimeout);
}

void Indicator::start(int delay, Scheme colorScheme)
{
	if (!isVisible()) return;

	setBarStyle(colorScheme);

	auto time_line = new QTimeLine(FILL_TIME, this);
	time_line->setFrameRange(MIN_RANGE, MAX_RANGE);
	connect(time_line, &QTimeLine::frameChanged, m_progressBar, &QProgressBar::setValue);
	connect(time_line, &QTimeLine::finished, time_line, &QTimeLine::deleteLater);

	auto slot = [&, time_line] {
		m_progressBar->show();
		m_holder->start(HOLD_TIME);
		time_line->start();
		m_resizer->start();
		};

	QTimer::singleShot(qBound(0, delay, MAX_DELAY), slot);
}

void Indicator::setBarStyle(Scheme colorScheme)
{
	auto qss = styleSheet(colorScheme);

	m_progressBar->setStyleSheet(qss);
}

QString Indicator::styleSheet(Scheme colorScheme) const
{
	auto qss = QString(QSS_FORMAT);

	switch (colorScheme) {
	case Scheme::Green:
		qss = qss.arg("00e878").arg("92ff00").arg("61e1bf").arg("00d4ff");
		break;
	default:
	case Scheme::Pastel:
		qss = qss.arg("7ce1f9").arg("3bb0f3").arg("9194f2").arg("f9b3f9");
		break;
	case Scheme::Red:
		qss = qss.arg("b43a3a").arg("fd1d1d").arg("fd6430").arg("fcb045");
		break;
	}

	return qss;
}

int Indicator::yPosition() const
{
	auto parent_window = qobject_cast<QMainWindow*>(parent());

	if (!parent_window)
		return 0;

	return (m_placement == Placement::Top)
		? parent_window->menuBar()->y() + parent_window->menuBar()->height()
		: parent_window->statusBar()->y() - height();
}

void Indicator::onHolderTimeout()
{
	m_progressBar->hide();
	m_progressBar->reset();
}

void Indicator::onResizerTimeout()
{
	if (!m_holder->isActive()) {
		m_resizer->stop();

		return;
	}

	auto parent_window = qobject_cast<QMainWindow*>(parent());
	if (!parent_window) return;

	move(0, yPosition());
	setFixedWidth(parent_window->width());
	raise();
}
