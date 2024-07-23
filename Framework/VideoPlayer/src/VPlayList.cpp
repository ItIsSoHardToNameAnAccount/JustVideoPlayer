#include "VPlayList.h"

#include <QEvent>
#include <QPainter>

VPlayList::VPlayList(QWidget* parent) :QTreeWidget(parent) 
{
	setStyleSheet("QTreeWidget { background-image: url(./Resources/playList.png); color: white; }");
}

void VPlayList::leaveEvent(QEvent* event)
{
	emit onMouseLeave();
	QTreeWidget::leaveEvent(event);
}

void VPlayList::toggleContextMenu(bool state)
{
	contextMenu = state;
}

bool VPlayList::isContextMenuOpen()
{
	return contextMenu;
}

void VPlayList::mousePressEvent(QMouseEvent* event)
{
	toggleContextMenu(false);
	QTreeWidget::mousePressEvent(event);
}