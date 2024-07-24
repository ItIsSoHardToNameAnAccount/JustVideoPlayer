#include "VButtonArea.h"

#include "JVolume.h"
#include "vlcPlayer.h"

#include <QPainter>

const int volumeAreaWidth = 300;
const int fullScreenButtonWidth = 200;

VButtonArea::VButtonArea(QWidget* parent) :QWidget(parent)
{
	setObjectName("buttonArea");

	mainLayout = new QHBoxLayout(this);

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
	mainLayout->addWidget(volumeArea);
	connect(volumeSlider, &QSlider::valueChanged, this, &VButtonArea::setVolume);

	videoPlayerControlButton = new QPushButton("Pause");
	mainLayout->addWidget(videoPlayerControlButton, 0, Qt::AlignCenter);
	connect(videoPlayerControlButton, &QPushButton::clicked, this, &VButtonArea::togglePlayPause);

	fullScreenButton = new QPushButton("FullScreen");
	fullScreenButton->setFixedWidth(fullScreenButtonWidth);
	mainLayout->addWidget(fullScreenButton);
	connect(fullScreenButton, &QPushButton::clicked, [=]() {emit fullScreenPressed(); });
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
		videoPlayerControlButton->setText("Play");
	}
	else if (state == MediaState::Playing)
	{
		videoPlayerControlButton->setText("Pause");
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