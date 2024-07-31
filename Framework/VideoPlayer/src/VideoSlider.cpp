#include "VideoSlider.h"

#include <QMouseEvent>
#include <QStyle>

void VideoSlider::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		int value = QStyle::sliderValueFromPosition(minimum(), maximum(), event->x(), width());
		setValue(value);
		emit positionChanged(value);
		event->accept();
	}
	QSlider::mousePressEvent(event);
}