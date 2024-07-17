#include "JVideoPlayerBase.h"

#include <QKeyEvent>
#include <QTimer>

JVideoPlayerBase::JVideoPlayerBase(QWidget* parent) :QWidget(parent) {}

void JVideoPlayerBase::playVideo(QTreeWidgetItem* item, int column)
{
	if (item->childCount() == 0)
	{
		QString QfilePath = item->data(0, Qt::UserRole).toString();
		std::string filePathStr = QfilePath.toStdString();
		std::replace(filePathStr.begin(), filePathStr.end(), '/', '\\');
		const char* c_filePath = filePathStr.c_str();
		int lastVideoTime = playVideoHandler(c_filePath, item);
		if (lastVideoTime != -1)
		{
			updateLastItem(lastVideoTime);
			lastItem = item;
		}
	}
}

MediaState JVideoPlayerBase::togglePlayPause()
{
	return player.togglePlayPause();
}

void JVideoPlayerBase::keyPressEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_Space)
	{
		togglePlayPause();
	}
	else if (event->key() == Qt::Key_Left)
	{
		seekForward(-baseForwardTime);
	}
	else if (event->key() == Qt::Key_Right)
	{
		seekForward(baseForwardTime);
	}
	else if (event->key() == Qt::Key_Up)
	{
		int currentVolume = JVolume::getVolume();
		setVolume(currentVolume + baseVolumeChangeValue);
	}
	else if (event->key() == Qt::Key_Down)
	{
		int currentVolume = JVolume::getVolume();
		setVolume(currentVolume - baseVolumeChangeValue);
	}

	QWidget::keyPressEvent(event);
}

void JVideoPlayerBase::seekForward(int forwardTime)
{
	player.seekForward(forwardTime);
}

void JVideoPlayerBase::setVolumeTipComponent(QLabel* volumeTip, QTimer* volumeTimer)
{
	volumeTip->setStyleSheet("QLabel { background-color : black; color : white; }");
	volumeTip->setAlignment(Qt::AlignCenter);
	volumeTip->setFixedSize(50, 50);
	volumeTip->move(10, 10);
	volumeTip->hide();
	volumeTimer->setSingleShot(true);
	connect(volumeTimer, &QTimer::timeout, volumeTip, &QLabel::hide);
}

void JVideoPlayerBase::updateLastItem(int time)
{
	lastItem->setData(0, Qt::UserRole + 1, time);
}