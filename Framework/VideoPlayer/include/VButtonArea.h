#ifndef JVP_ButtonArea_H
#define JVP_ButtonArea_H

#include <QWidget>
#include <QBoxLayout>
#include <QLabel>
#include <QSlider>
#include <QPushButton>

#include "VideoSlider.h"

class VButtonArea :public QWidget
{
	Q_OBJECT
public:
	VButtonArea(QWidget* parent = nullptr);
	void setVideoDuration();
	void setVideoSlider();
	void sliderSeekForward(int forwardTime);
public slots:
	void setVolumeSlider(int value);
	void togglePlayPause();
	void updateSlider();
protected:
	void leaveEvent(QEvent* event) override;
	void paintEvent(QPaintEvent* event) override;
private slots:
	void setVolume(int value);
	void setVideoPosition(int position);
	void onSliderPressed();
	void onSliderReleased();
private:
	QVBoxLayout* mainLayout;

	QWidget* videoSliderArea;
	QHBoxLayout* videoSliderLayout;
	VideoSlider* videoSlider;

	QWidget* buttonArea;
	QHBoxLayout* buttonLayout;
	QWidget* volumeArea;
	QHBoxLayout* volumeLayout;
	QLabel* volumeLabel;
	QSlider* volumeSlider;

	QPushButton* videoPlayerControlButton;

	QPushButton* fullScreenButton;
signals:
	void tipVolume();
	void fullScreenPressed();
	void onMouseLeave();
	void videoPlaying();
	void videoPaused();
	void sliderPressed();
	void sliderReleased();
};

#endif // !JVP_ButtonArea_H