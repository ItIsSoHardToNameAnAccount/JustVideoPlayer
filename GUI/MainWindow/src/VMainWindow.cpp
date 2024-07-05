#include "VMainWindow.h"
#include "vlcPlayer.h"

#include <QGuiApplication>
#include <QRect>
#include <QMenu>
#include <QFileDialog>
#include <QKeyEvent>
#include <QTimer>

const QSize windowDefaultSize(1600, 960);
const int volumeAreaMaxWidth = 400;

VMainWindow::VMainWindow(QWidget* parent) :JVideoPlayerBase(parent)
{
	setWindowTitle("JustVideoPlayer");

	screen = QGuiApplication::primaryScreen();
	setWindowToCentral();

	mainLayout = new QVBoxLayout(this);
	mainContent = new QFrame;
	topLayout = new QHBoxLayout(mainContent);

	setVideoWidget();
	setPlayList();
	mainLayout->addWidget(mainContent);

	setButtonArea();
	mainLayout->addWidget(buttonArea);

	volumeTip = new QLabel("100", this);
	volumeTip->setStyleSheet("QLabel { background-color : black; color : white; }");
	volumeTip->setAlignment(Qt::AlignCenter);
	volumeTip->setFixedSize(50, 50);
	volumeTip->move(10, 10);
	volumeTip->hide();
	volumeTimer = new QTimer(this);
	volumeTimer->setSingleShot(true);
	connect(volumeTimer, &QTimer::timeout, volumeTip, &QLabel::hide);

	show();
}

void VMainWindow::setWindowToCentral()
{
	resize(windowDefaultSize);
	QRect screenGeometry = screen->availableGeometry();
	int x = (screenGeometry.width() - width()) / 2;
	int y = (screenGeometry.height() - height()) / 2;
	move(x, y);
}

void VMainWindow::setVideoWidget()
{
	videoWidget = new JVideoWidget;
	topLayout->addWidget(videoWidget);
}

void VMainWindow::setPlayList()
{
	playList->setHeaderHidden(true);
	playList->setMaximumWidth(playListMaxWidth);
	playList->setContextMenuPolicy(Qt::CustomContextMenu);
	topLayout->addWidget(playList);
	connect(playList, &QTreeWidget::customContextMenuRequested, this, &VMainWindow::showContextMenu);
	connect(playList, &QTreeWidget::itemDoubleClicked, this, &VMainWindow::playVideo);
}

void VMainWindow::setButtonArea()
{
	buttonArea = new QFrame;
	buttonAreaLayout = new QHBoxLayout(buttonArea);
	videoPlayerControlButton = new QPushButton("Pause");
	buttonAreaLayout->addWidget(videoPlayerControlButton, 0, Qt::AlignCenter);
	connect(videoPlayerControlButton, &QPushButton::clicked, this, &VMainWindow::togglePlayPause);

	volumeArea = new QWidget;
	volumeLayout = new QHBoxLayout(volumeArea);
	volumeLabel = new QLabel("volume");
	volumeSlider = new QSlider(Qt::Horizontal);
	volumeSlider->setRange(0, 100);
	volumeSlider->setValue(100);
	volumeLayout->addWidget(volumeLabel);
	volumeLayout->addWidget(volumeSlider);
	volumeArea->setMaximumWidth(volumeAreaMaxWidth);
	buttonAreaLayout->addWidget(volumeArea);
	connect(volumeSlider, &QSlider::valueChanged, this, &VMainWindow::setVolume);
}

void VMainWindow::showContextMenu(const QPoint& pos)
{
	logger.logDebug("ContextMenu Requested");
	QTreeWidgetItem* item = playList->itemAt(pos);
	QMenu contextMenu(this);

	QAction* addAction = contextMenu.addAction("Add Video/Folder");
	connect(addAction, &QAction::triggered, this, &VMainWindow::addVideo);

	if (item)
	{
		QAction* removeAction = contextMenu.addAction("Remove Video/Folder");
		connect(removeAction, &QAction::triggered, this, [=]() {removeVideo(item); });
	}

	contextMenu.exec(playList->mapToGlobal(pos));
}

void VMainWindow::addVideo()
{
	QStringList fileNames = QFileDialog::getOpenFileNames(this, "Open Video Files or Folder", "", "Video Files (*.mp4 *.avi *.mkv);;All Files (*)");
	if (!fileNames.isEmpty())
	{
		foreach(const QString& filePath, fileNames)
		{
			QFileInfo fileInfo(filePath);
			if (fileInfo.isDir())
			{
				QDir directory(filePath);
				QStringList videoFiles = directory.entryList(QStringList() << "*.mp4" << "*.avi" << "*.mkv", QDir::Files);
				if (!videoFiles.isEmpty())
				{
					QTreeWidgetItem* folderItem = new QTreeWidgetItem(playList);
					folderItem->setText(0, directory.dirName());
					foreach(const QString& filename, videoFiles)
					{
						QTreeWidgetItem* item = new QTreeWidgetItem(folderItem);
						item->setText(0, filename);
						item->setData(0, Qt::UserRole, directory.absoluteFilePath(filename));
					}
				}
			}
			else
			{
				QTreeWidgetItem* fileItem = new QTreeWidgetItem(playList);
				fileItem->setText(0, fileInfo.fileName());
				fileItem->setData(0, Qt::UserRole, filePath);
			}
		}
	}
}

void VMainWindow::removeVideo(QTreeWidgetItem* item)
{
	if (item->parent())
	{
		item->parent()->removeChild(item);
	}
	else
	{
		int index = playList->indexOfTopLevelItem(item);
		playList->takeTopLevelItem(index);
	}
	delete item;
}

void VMainWindow::playVideoHandler(const char* filePath)
{
	player.play(filePath, reinterpret_cast<void*>(videoWidget->winId()));
}

void VMainWindow::keyPressEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_Space)
	{
		togglePlayPause();
	}
	else if (event->key() == Qt::Key_Left)
	{
		seek(Direction::backward);
	}
	else if (event->key() == Qt::Key_Right)
	{
		seek(Direction::forward);
	}
	else if (event->key() == Qt::Key_Up)
	{
		setVolumeSlider(10);
	}
	else if (event->key() == Qt::Key_Down)
	{
		setVolumeSlider(-10);
	}

	QWidget::keyPressEvent(event);
}

void VMainWindow::togglePlayPause()
{
	if (!libvlcMediaPlayer)
	{
		return;
	}
	libvlc_state_t state = libvlc_media_player_get_state(libvlcMediaPlayer);
	if (state == libvlc_Playing)
	{
		videoPlayerControlButton->setText("Play");
		libvlc_media_player_pause(libvlcMediaPlayer);
	}
	else if (state == libvlc_Paused)
	{
		videoPlayerControlButton->setText("Pause");
		libvlc_media_player_play(libvlcMediaPlayer);
	}
}

void VMainWindow::seek(Direction direction)
{
	if (!libvlcMediaPlayer)
	{
		return;
	}
	libvlc_time_t currentTime = libvlc_media_player_get_time(libvlcMediaPlayer);
	if (direction == Direction::backward)
	{
		libvlc_media_player_set_time(libvlcMediaPlayer, currentTime - 5000);
	}
	else if (direction == Direction::forward)
	{
		libvlc_media_player_set_time(libvlcMediaPlayer, currentTime + 5000);
	}
}

void VMainWindow::setVolume(int value)
{
	if (!libvlcMediaPlayer)
	{
		return;
	}

	libvlc_audio_set_volume(libvlcMediaPlayer, value);

	showVolumeTip(value);
}

void VMainWindow::setVolumeSlider(int value)
{
	int volumeValue = volumeSlider->value();
	volumeValue += value;
	volumeValue = std::max(std::min(100, volumeValue), 0);
	volumeSlider->setValue(volumeValue);
}

void VMainWindow::showVolumeTip(int value)
{
	volumeTip->setText(QString("%1").arg(value));
	volumeTip->show();
	volumeTimer->start(1000);
}

void VMainWindow::toggleVideoFullScreen()
{
	logger.logDebug("Video Widget double clicked.");
	/*if (isFullScreen)
	{
		playList->show();
		buttonArea->show();
		isFullScreen = false;
	}
	else
	{
		playList->hide();
		buttonArea->hide();
		isFullScreen = true;
	}*/
}