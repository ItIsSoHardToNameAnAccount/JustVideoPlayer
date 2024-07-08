#include "VMainWindow.h"

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

	volumeTip = new QLabel(this);
	volumeTimer = new QTimer(this);
	setVolumeTipComponent(volumeTip, volumeTimer);

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
	videoWidget = new QWidget;
	topLayout->addWidget(videoWidget);
}

void VMainWindow::setPlayList()
{
	playList = new QTreeWidget;
	playList->setHeaderHidden(true);
	playList->setMaximumWidth(playListMaxWidth);
	playList->setContextMenuPolicy(Qt::CustomContextMenu);
	topLayout->addWidget(playList);
	JPlayListData::load(playList);
	connect(playList, &QTreeWidget::customContextMenuRequested, this, &VMainWindow::showContextMenu);
	connect(playList, &QTreeWidget::itemDoubleClicked, this, &VMainWindow::playVideo);
}

void VMainWindow::setButtonArea()
{
	buttonArea = new QFrame;
	buttonAreaLayout = new QHBoxLayout(buttonArea);

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

	videoPlayerControlButton = new QPushButton("Pause");
	buttonAreaLayout->addWidget(videoPlayerControlButton, 0, Qt::AlignCenter);
	connect(videoPlayerControlButton, &QPushButton::clicked, this, &VMainWindow::v_togglePlayPause);

	fullScreenButton = new QPushButton("FullScreen");
	buttonAreaLayout->addWidget(fullScreenButton);
	connect(fullScreenButton, &QPushButton::clicked, this, &VMainWindow::setFullScreen);
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
		v_togglePlayPause();
	}
	else if (event->key() == Qt::Key_Up)
	{
		setVolumeSlider(10);
	}
	else if (event->key() == Qt::Key_Down)
	{
		setVolumeSlider(-10);
	}
	else
	{
		JVideoPlayerBase::keyPressEvent(event);
		return;
	}
	QWidget::keyPressEvent(event);
}

void VMainWindow::v_togglePlayPause()
{
	MediaState state = togglePlayPause();
	if (state == MediaState::Paused)
	{
		videoPlayerControlButton->setText("Play");
	}
	else if (state == MediaState::Playing)
	{
		videoPlayerControlButton->setText("Pause");
	}
}

void VMainWindow::setVolumeSlider(int value)
{
	int volumeValue = volumeSlider->value();
	volumeValue += value;
	volumeValue = std::max(std::min(100, volumeValue), 0);
	volumeSlider->setValue(volumeValue);
}

void VMainWindow::setFullScreen()
{
	JPlayListData::sync(playList);
	hide();
}

void VMainWindow::setVideoWindow(JVideoWindow* videoWindow)
{
	connect(fullScreenButton, &QPushButton::clicked, videoWindow, &JVideoWindow::setFullScreen);
	connect(videoWindow, &JVideoWindow::onWidgetDoubleClicked, this, &VMainWindow::setNormalScreen);
	connect(this, &VMainWindow::programClosed, videoWindow, &JVideoWindow::closeHiddenWindow);
}

void VMainWindow::setNormalScreen()
{
	show();
	player.switchOutputWindow(reinterpret_cast<void*>(videoWidget->winId()));
}

void VMainWindow::closeEvent(QCloseEvent* event)
{
	emit programClosed();
	JVideoPlayerBase::closeEvent(event);
}

void VMainWindow::tipCurrentVolume(int currentVolume)
{
	volumeTip->setText(QString("%1").arg(currentVolume));
	volumeTip->show();
	volumeTimer->start(volumeTipConstantTime);
}

void VMainWindow::setVolume(int value)
{
	JVolume::setVolume(value);
	tipCurrentVolume(value);
}