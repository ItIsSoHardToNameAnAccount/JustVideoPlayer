#ifndef JVP_VideoSlider_H
#define JVP_VideoSlider_H

#include <QSlider>

class VideoSlider :public QSlider
{
	Q_OBJECT
public:
	using QSlider::QSlider;
protected:
	void mousePressEvent(QMouseEvent* event) override;
signals:
	void positionChanged(int value);
};

#endif // !JVP_VideoSlider_H