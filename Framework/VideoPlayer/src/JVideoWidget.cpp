#include "JVideoWidget.h"

#include <QMouseEvent>

JVideoWidget::JVideoWidget(QWidget* parent) :QWidget(parent)
{
	//
}

void JVideoWidget::mouseDoubleClickEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		emit onWidgetDoubleClicked();
	}
	QWidget::mouseDoubleClickEvent(event);
}