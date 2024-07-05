#include "JVideoWindow.h"
#include "vlcPlayer.h"

JVideoWindow::JVideoWindow(QWidget* parent) :JVideoPlayerBase(parent)
{
	showFullScreen();
	setPlayList();
}

void JVideoWindow::setPlayList()
{
	playList->setHeaderHidden(true);
	playList->setMaximumWidth(playListMaxWidth);
	playList->setFixedHeight(this->height());
	int pos_x = this->width() - playList->width();
	int pos_y = this->pos().y();
	playList->move(pos_x, pos_y);
	playList->hide();
	connect(playList, &QTreeWidget::itemDoubleClicked, this, &JVideoWindow::playVideo);
}

void JVideoWindow::playVideoHandler(const char* filePath)
{
	player.play(filePath, reinterpret_cast<void*>(this->winId()));
}