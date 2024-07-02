#include "VMainWindow.h"

#include <QGuiApplication>
#include <QRect>
#include <QMenu>
#include <QFileDialog>
#include <QKeyEvent>

const QSize windowDefaultSize(1600, 960);
const int playListMaxWidth = 400;

VMainWindow::VMainWindow(QWidget* parent) :QWidget(parent)
{
	screen = QGuiApplication::primaryScreen();
	setWindowToCentral();

	mainLayout = new QVBoxLayout(this);
	mainContent = new QFrame;
	topLayout = new QHBoxLayout(mainContent);

	setVideoPlayer();
	setPlayList();
	mainLayout->addWidget(mainContent);

	setButtonArea();
	mainLayout->addWidget(buttonArea);

	show();
}

VMainWindow::~VMainWindow()
{
	if (libvlcMediaPlayer)
	{
		libvlc_media_player_stop(libvlcMediaPlayer);
		libvlc_media_player_release(libvlcMediaPlayer);
	}
	if (libvlcInstance)
	{
		libvlc_release(libvlcInstance);
	}
}

void VMainWindow::setWindowToCentral()
{
	resize(windowDefaultSize);
	QRect screenGeometry = screen->availableGeometry();
	int x = (screenGeometry.width() - width()) / 2;
	int y = (screenGeometry.height() - height()) / 2;
	move(x, y);
}

void VMainWindow::setVideoPlayer()
{
	videoWidget = new QWidget;
	topLayout->addWidget(videoWidget);
	
	libvlcInstance = libvlc_new(0, nullptr);
	if (!libvlcInstance)
	{
		logger.logDebug("libvlc_new failed!");
		return;
	}
	libvlcMediaPlayer = libvlc_media_player_new(libvlcInstance);
	if (!libvlcMediaPlayer)
	{
		logger.logDebug("libvlc_media_player_new failed!");
		libvlc_release(libvlcInstance);
	}
}

void VMainWindow::setPlayList()
{
	playList = new VPlayList;
	playList->setHeaderHidden(true);
	playList->setMaximumWidth(playListMaxWidth);
	playList->setContextMenuPolicy(Qt::CustomContextMenu);
	topLayout->addWidget(playList);
	connect(playList, &VPlayList::customContextMenuRequested, this, &VMainWindow::showContextMenu);
	connect(playList, &VPlayList::itemDoubleClicked, this, &VMainWindow::playVideo);
}

void VMainWindow::setButtonArea()
{
	buttonArea = new QFrame;
	buttonAreaLayout = new QHBoxLayout(buttonArea);
	videoPlayerControlButton = new QPushButton("Pause");
	buttonAreaLayout->addWidget(videoPlayerControlButton, 0, Qt::AlignCenter);
	connect(videoPlayerControlButton, &QPushButton::clicked, this, &VMainWindow::togglePlayPause);
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
				startVideo(filePath);
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
		QString filePath = item->data(0, Qt::UserRole).toString();
		startVideo(filePath);
	}
}

void VMainWindow::startVideo(const QString& filePath)
{
	std::string filePathStr = filePath.toStdString();
	std::replace(filePathStr.begin(), filePathStr.end(), '/', '\\');
	const char* c_filePath = filePathStr.c_str();
	libvlc_media_t* media = libvlc_media_new_path(libvlcInstance, c_filePath);
	if (!media)
	{
		logger.logDebug("libvlc_media_new_path failed!");
		return;
	}
	libvlc_media_player_set_media(libvlcMediaPlayer, media);
	libvlc_media_release(media);
	
	libvlc_media_player_set_hwnd(libvlcMediaPlayer, reinterpret_cast<void*>(videoWidget->winId()));
	if (libvlc_media_player_play(libvlcMediaPlayer) == -1)
	{
		logger.logDebug("libvlc_media_player_play failed!");
	}
}

void VMainWindow::keyPressEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_Space)
	{
		togglePlayPause();
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