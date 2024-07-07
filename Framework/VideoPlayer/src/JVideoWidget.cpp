#include "JVideoWidget.h"

#include <QMouseEvent>

JVideoWidgetOverlay::JVideoWidgetOverlay(QWidget* parent) :QWidget(parent)
{
	setAttribute(Qt::WA_TransparentForMouseEvents);
	setAttribute(Qt::WA_NoSystemBackground);
	setWindowFlag(Qt::FramelessWindowHint);
	setAttribute(Qt::WA_TranslucentBackground);
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