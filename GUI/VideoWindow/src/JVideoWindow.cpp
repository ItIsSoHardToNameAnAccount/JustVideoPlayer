#include "JVideoWindow.h"
#include "vlcPlayer.h"

#include <QMouseEvent>
#include <QTimer>

JVideoWindow::JVideoWindow(QWidget* parent) :JVideoPlayerBase(parent)
{
	setPlayList();

	volumeTip = new QLabel(this);
	volumeTimer = new QTimer(this);
	setVolumeTipComponent(volumeTip, volumeTimer);
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
	player.switchOutputWindow(reinterpret_cast<void*>(this->winId()));
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

void JVideoWindow::reciveDoubleClickSignal()
{
	setNormalScreen();
	emit onWidgetDoubleClicked();
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

void JVideoWindow::keyPressEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_Escape)
	{
		emit onWidgetDoubleClicked();
		setNormalScreen();
	}
	JVideoPlayerBase::keyPressEvent(event);
}