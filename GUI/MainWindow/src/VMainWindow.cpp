#include "VMainWindow.h"

#include <QGuiApplication>
#include <QRect>
#include <QMenu>
#include <QFileDialog>
#include <QKeyEvent>
#include <QTimer>

#include "JPlayListData.h"

const QSize windowDefaultSize(1600, 960);
const int playListMaxWidth = 400;
const int volumeAreaMaxWidth = 400;

VMainWindow::VMainWindow(QWidget* parent) :QWidget(parent)
{
	setWindowTitle("JustVideoPlayer");

	screen = QGuiApplication::primaryScreen();
	setWindowToCentral();

	videoWidget = new QWidget(this);
	videoWidget->resize(this->size());
	videoWidget->setStyleSheet("background-color: black;");

	setPlayList();

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

void VMainWindow::setPlayList()
{
	playList = new QTreeWidget;
	playList->setHeaderHidden(true);
	playList->setMaximumWidth(playListMaxWidth);
	playList->setContextMenuPolicy(Qt::CustomContextMenu);
	JPlayListData::load(playList);
	connect(playList, &QTreeWidget::customContextMenuRequested, this, &VMainWindow::showContextMenu);
	connect(playList, &QTreeWidget::itemDoubleClicked, this, &VMainWindow::playVideo);
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
		foreach(const QString & filePath, fileNames)
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
					foreach(const QString & filename, videoFiles)
					{
						QTreeWidgetItem* item = new QTreeWidgetItem(folderItem);
						item->setText(0, filename);
						item->setData(0, Qt::UserRole, directory.absoluteFilePath(filename));
						item->setData(0, Qt::UserRole + 1, 0);
					}
				}
			}
			else
			{
				QTreeWidgetItem* fileItem = new QTreeWidgetItem(playList);
				fileItem->setText(0, fileInfo.fileName());
				fileItem->setData(0, Qt::UserRole, filePath);
				fileItem->setData(0, Qt::UserRole + 1, 0);
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

void VMainWindow::playVideo(QTreeWidgetItem* item, int column)
{
	if (item->childCount() == 0)
	{
		QString QfilePath = item->data(0, Qt::UserRole).toString();
		std::string filePathStr = QfilePath.toStdString();
		std::replace(filePathStr.begin(), filePathStr.end(), '/', '\\');
		const char* c_filePath = filePathStr.c_str();
		int lastVideoTime = playVideoHandler(c_filePath, item);
		if (lastVideoTime != -1)
		{
			updateLastItem(lastVideoTime);
			lastItem = item;
		}
	}
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

int VMainWindow::playVideoHandler(const char* filePath, QTreeWidgetItem* item)
{
	MediaData mediaData(item);
	return player.play(filePath, mediaData, reinterpret_cast<void*>(videoWidget->winId()));
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
	else if (event->key() == Qt::Key_Escape)
	{
		setNormalScreen();
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
	//JPlayListData::sync(playList);
	//hide();
	//emit windowHidden();
	playList->hide();
	buttonArea->hide();
	showFullScreen();
}

void VMainWindow::setVideoWindow(JVideoWindow* videoWindow)
{
	connect(this, &VMainWindow::windowHidden, videoWindow, &JVideoWindow::setFullScreen);
	connect(videoWindow, &JVideoWindow::onWidgetDoubleClicked, this, &VMainWindow::setNormalScreen);
	connect(this, &VMainWindow::programClosed, videoWindow, &JVideoWindow::closeHiddenWindow);
}

void VMainWindow::setNormalScreen()
{
	//JPlayListData::load(playList);
	//show();
	//player.switchOutputWindow(reinterpret_cast<void*>(videoWidget->winId()));
	playList->show();
	buttonArea->show();
	showNormal();
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