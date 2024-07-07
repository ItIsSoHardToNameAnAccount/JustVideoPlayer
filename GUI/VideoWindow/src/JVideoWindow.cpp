#include "JVideoWindow.h"
#include "vlcPlayer.h"

#include <QMouseEvent>
#include <QTimer>

JVideoWindow::JVideoWindow(QWidget* parent) :JVideoPlayerBase(parent)
{
	setPlayList();
	
	setVideoWidgetOverlay();

	volumeTip = new QLabel(this);
	volumeTimer = new QTimer(this);
	setVolumeTipComponent(volumeTip, volumeTimer);

	connect(videoWidgetOverlay, &JVideoWidgetOverlay::onWidgetDoubleClicked, this, &JVideoWindow::reciveDoubleClickSignal);
}

void JVideoWindow::setPlayList()
{
	playList = new QTreeWidget(this);
	playList->setHeaderHidden(true);
	playList->setMaximumWidth(playListMaxWidth);
	playList->setFixedHeight(this->height());
	int pos_x = this->width() - playList->width();
	int pos_y = this->pos().y();
	playList->move(pos_x, pos_y);
	playList->hide();
	connect(playList, &QTreeWidget::itemDoubleClicked, this, &JVideoWindow::playVideo);
	this->installEventFilter(this);
}

void JVideoWindow::playVideoHandler(const char* filePath)
{
	player.play(filePath, reinterpret_cast<void*>(this->winId()));
}

void JVideoWindow::setNormalScreen()
{
	hide();
}

void JVideoWindow::setFullScreen()
{
	JPlayListData::load(playList);
	showFullScreen();
	player.setOutputWindow(reinterpret_cast<void*>(this->winId()));
	logger.logDebug("Size of overlay is ", videoWidgetOverlay->size().width(), videoWidgetOverlay->size().height());
}

bool JVideoWindow::eventFilter(QObject* watched, QEvent* event)
{
	if (watched == this && event->type() == QEvent::MouseMove)
	{
		QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
		if (mouseEvent->pos().x() >= this->width() - 10)
		{
			playList->show();
		}
		else if (mouseEvent->pos().x() < this->width() - playList->width() - 10)
		{
			playList->hide();
		}
	}
	return JVideoPlayerBase::eventFilter(watched, event);
}

void JVideoWindow::closeHiddenWindow()
{
	logger.logDebug("close hidden window.");
	close();
}

void JVideoWindow::setVideoWidgetOverlay()
{
	videoWidgetOverlay = new JVideoWidgetOverlay(this);
	videoWidgetOverlay->resize(this->size());
	videoWidgetOverlay->move(this->pos());
}

void JVideoWindow::reciveDoubleClickSignal()
{
	setNormalScreen();
	emit onWidgetDoubleClicked();
}

void JVideoWindow::resizeEvent(QResizeEvent* event)
{
	JVideoPlayerBase::resizeEvent(event);
	videoWidgetOverlay->resize(this->size());
	videoWidgetOverlay->move(this->pos());
	playList->setFixedHeight(this->height() - 10);
}

void JVideoWindow::tipCurrentVolume(int currentVolume)
{
	volumeTip->setText(QString("%1").arg(currentVolume));
	volumeTip->show();
	volumeTimer->start(volumeTipConstantTime);
}

void JVideoWindow::setVolume(int value)
{
	JVolume::setVolume(value);
	tipCurrentVolume(value);
}