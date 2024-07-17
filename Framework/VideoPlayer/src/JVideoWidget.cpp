#include "JVideoWidget.h"

#include <QMouseEvent>

JVideoWidgetOverlay::JVideoWidgetOverlay(QWidget* parent) :QWidget(parent)
{
	setWindowOpacity(0.5);
	setAttribute(Qt::WA_TranslucentBackground);
	setWindowFlags(Qt::FramelessWindowHint);
}

void JVideoWidgetOverlay::mouseDoubleClickEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		logger.logDebug("Trying to toggle full/normal screen.");
		emit onWidgetDoubleClicked();
	}
	QWidget::mouseDoubleClickEvent(event);
}