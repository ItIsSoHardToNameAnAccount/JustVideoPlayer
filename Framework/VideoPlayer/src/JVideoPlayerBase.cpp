#include "JVideoPlayerBase.h"

#include <QKeyEvent>

JVideoPlayerBase::JVideoPlayerBase(QWidget* parent) :QWidget(parent)
{
	playList = new QTreeWidget(this);
}

void JVideoPlayerBase::playVideo(QTreeWidgetItem* item, int column)
{
	if (item->childCount() == 0)
	{
		QString filePath = item->data(0, Qt::UserRole).toString();
		std::string filePathStr = filePath.toStdString();
		std::replace(filePathStr.begin(), filePathStr.end(), '/', '\\');
		const char* c_filePath = filePathStr.c_str();
		playVideoHandler(c_filePath);
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
		setVolumeSlider(10);
	}
	else if (event->key() == Qt::Key_Down)
	{
		setVolumeSlider(-10);
	}

	QWidget::keyPressEvent(event);
}

void JVideoPlayerBase::seekForward(int forwardTime)
{
	player.seekForward(forwardTime);
}