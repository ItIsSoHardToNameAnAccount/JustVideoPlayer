#include "VButtonArea.h"

#include "JVolume.h"
#include "vlcPlayer.h"

#include <QPainter>

const int volumeAreaWidth = 300;
const int fullScreenButtonWidth = 200;
const QSize controlButtonSize(75, 75);

VButtonArea::VButtonArea(QWidget* parent) :QWidget(parent)
{
	setObjectName("buttonArea");

	mainLayout = new QVBoxLayout(this);

	videoSliderArea = new QWidget;
	videoSliderLayout = new QHBoxLayout(videoSliderArea);
	videoSlider = new QSlider(Qt::Horizontal);
	videoSlider->setRange(0, 0);
	videoSliderLayout->addWidget(videoSlider);
	connect(videoSlider, &QSlider::sliderMoved, this, &VButtonArea::setVideoPosition);

	mainLayout->addWidget(videoSliderArea);

	buttonArea = new QWidget;
	buttonLayout = new QHBoxLayout(buttonArea);
	volumeArea = new QWidget;
	volumeLayout = new QHBoxLayout(volumeArea);
	volumeLabel = new QLabel("Volume");
	volumeLabel->setStyleSheet("QLabel { color : white; }");
	volumeSlider = new QSlider(Qt::Horizontal);
	volumeSlider->setRange(0, 100);
	volumeSlider->setValue(JVolume::getVolume());
	volumeLayout->addWidget(volumeLabel);
	volumeLayout->addWidget(volumeSlider);
	volumeArea->setFixedWidth(volumeAreaWidth);
	buttonLayout->addWidget(volumeArea);
	connect(volumeSlider, &QSlider::valueChanged, this, &VButtonArea::setVolume);

	videoPlayerControlButton = new QPushButton;
	videoPlayerControlButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
	videoPlayerControlButton->setFixedSize(controlButtonSize);
	buttonLayout->addWidget(videoPlayerControlButton, 0, Qt::AlignCenter);
	connect(videoPlayerControlButton, &QPushButton::clicked, this, &VButtonArea::togglePlayPause);

	fullScreenButton = new QPushButton("FullScreen");
	fullScreenButton->setFixedWidth(fullScreenButtonWidth);
	buttonLayout->addWidget(fullScreenButton);
	connect(fullScreenButton, &QPushButton::clicked, [=]() {emit fullScreenPressed(); });

	mainLayout->addWidget(buttonArea);
}

void VButtonArea::setVolume(int value)
{
	JVolume::setVolume(value);
	emit tipVolume();
}

void VButtonArea::setVolumeSlider(int value)
{
	int volumeValue = volumeSlider->value();
	volumeValue += value;
	volumeValue = std::max(std::min(100, volumeValue), 0);
	volumeSlider->setValue(volumeValue);
}

void VButtonArea::togglePlayPause()
{
	MediaState state = player.togglePlayPause();
	if (state == MediaState::Paused)
	{
		videoPlayerControlButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
		emit videoPaused();
	}
	else if (state == MediaState::Playing)
	{
		videoPlayerControlButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
		emit videoPlaying();
	}
}

void VButtonArea::leaveEvent(QEvent* event)
{
	emit onMouseLeave();
	QWidget::leaveEvent(event);
}

void VButtonArea::paintEvent(QPaintEvent* event)
{
	// Use same background with playList
	QPainter painter(this);
	QPixmap pixmap("./Resources/playList.png");
	painter.drawPixmap(0, 0, width(), height(), pixmap);
	QWidget::paintEvent(event);
}

void VButtonArea::setVideoDuration()
{
	int duration = player.getDuration();
	videoSlider->setRange(0, duration);
}

void VButtonArea::setVideoSlider()
{
	int currentTime = player.getCurrentTime();
	videoSlider->setValue(currentTime);
}

void VButtonArea::updateSlider()
{
	setVideoSlider();
}

void VButtonArea::setVideoPosition(int position)
{
	emit videoPaused();
	player.setVideoPosition(position);
	emit videoPlaying();
}

void VButtonArea::sliderSeekForward(int forwardTime)
{
	int currentValue = videoSlider->value();
	videoSlider->setValue(currentValue + forwardTime / 1000);
}