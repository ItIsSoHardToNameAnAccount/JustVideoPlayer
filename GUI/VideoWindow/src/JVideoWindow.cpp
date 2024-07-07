#include "JVideoWindow.h"
#include "vlcPlayer.h"

#include <QMouseEvent>

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

void JVideoWindow::mouseDoubleClickEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		setNormalScreen();
		emit onWidgetDoubleClicked();
	}
	JVideoPlayerBase::mouseDoubleClickEvent(event);
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
}