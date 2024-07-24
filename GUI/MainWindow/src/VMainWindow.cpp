#include "VMainWindow.h"

#include <QGuiApplication>
#include <QRect>
#include <QMenu>
#include <QFileDialog>
#include <QKeyEvent>
#include <QTimer>
#include <QPainter>

#include "JPlayListData.h"
#include "JVolume.h"

const QSize windowDefaultSize(1600, 960);
const int playListWidth = 400;
const int volumeTipConstantTime = 1000;
const int baseForwardTime = 5000; // 5s
const int baseVolumeChangeValue = 10;
const int buttonAreaHeight = 200;

VMainWindow::VMainWindow(QWidget* parent) :QWidget(parent)
{
	setWindowTitle("JustVideoPlayer");
	setMouseTracking(true);

	screen = QGuiApplication::primaryScreen();
	setWindowToCentral();

	setVideoWidget();
	setPlayList();
	setButtonArea();
	setVolumeTipComponent();

	show();
	setFocus();
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
	videoWidget = new QWidget(this);
	//videoWidget->setStyleSheet("background-color: white;");
	resizeVideoWidget(size());
}

void VMainWindow::setPlayList()
{
	playList = new VPlayList(this);
	playList->setHeaderHidden(true);
	playList->setFixedWidth(playListWidth);
	playList->setContextMenuPolicy(Qt::CustomContextMenu);
	JPlayListData::load(playList);
	connect(playList, &VPlayList::customContextMenuRequested, this, &VMainWindow::showContextMenu);
	connect(playList, &VPlayList::itemDoubleClicked, this, &VMainWindow::playVideo);
	connect(playList, &VPlayList::onMouseLeave, this, &VMainWindow::hidePlayList);

	playList->hide();
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

	playList->toggleContextMenu(true);
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
	playList->toggleContextMenu(false);
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

int VMainWindow::playVideoHandler(const char* filePath, QTreeWidgetItem* item)
{
	MediaData mediaData(item);
	return player.play(filePath, mediaData, reinterpret_cast<void*>(videoWidget->winId()));
}

void VMainWindow::updateLastItem(int time)
{
	lastItem->setData(0, Qt::UserRole + 1, time);
}

void VMainWindow::setButtonArea()
{
	buttonArea = new VButtonArea(this);

	connect(buttonArea, &VButtonArea::tipVolume, this, &VMainWindow::tipCurrentVolume);
	connect(this, &VMainWindow::volumeKeyPressed, buttonArea, &VButtonArea::setVolumeSlider);
	connect(this, &VMainWindow::spaceKeyPressed, buttonArea, &VButtonArea::togglePlayPause);
	connect(buttonArea, &VButtonArea::fullScreenPressed, this, &VMainWindow::setFullScreen);
	connect(buttonArea, &VButtonArea::onMouseLeave, this, &VMainWindow::hideButtonArea);

	buttonArea->setFixedHeight(buttonAreaHeight);
	buttonArea->hide();
}

void VMainWindow::tipCurrentVolume()
{
	int currentVolume = JVolume::getVolume();
	volumeTip->setText(QString("%1").arg(currentVolume));
	volumeTip->show();
	volumeTimer->start(volumeTipConstantTime);
}

void VMainWindow::setFullScreen()
{
	// ToDo: reset layout
}

void VMainWindow::setVolumeTipComponent()
{
	volumeTip = new QLabel(this);
	volumeTimer = new QTimer(this);
	volumeTip->setStyleSheet("QLabel { background-color : black; color : white; }");
	volumeTip->setAlignment(Qt::AlignCenter);
	volumeTip->setFixedSize(50, 50);
	volumeTip->move(10, 10);
	volumeTip->hide();
	volumeTimer->setSingleShot(true);
	connect(volumeTimer, &QTimer::timeout, volumeTip, &QLabel::hide);
}

void VMainWindow::keyPressEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_Space)
	{
		emit spaceKeyPressed();
	}
	else if (event->key() == Qt::Key_Left)
	{
		seekForward(-baseForwardTime);
	}
	else if (event->key() == Qt::Key_Right)
	{
		seekForward(baseForwardTime);
	}
	else if (event->key() == Qt::Key_Up)
	{
		emit volumeKeyPressed(baseVolumeChangeValue);
	}
	else if (event->key() == Qt::Key_Down)
	{
		emit volumeKeyPressed(-baseVolumeChangeValue);
	}

	QWidget::keyPressEvent(event);
}

void VMainWindow::seekForward(int forwardTime)
{
	player.seekForward(forwardTime);
}

void VMainWindow::focusOutEvent(QFocusEvent* event)
{
	if (event->reason() != Qt::PopupFocusReason)
	{
		QTimer::singleShot(500, this, SLOT(setFocusToWindow()));
	}
	QWidget::focusOutEvent(event);
}

void VMainWindow::setFocusToWindow()
{
	setFocus();
	logger.logDebug("Focus main window.");
}

void VMainWindow::resizeEvent(QResizeEvent* event)
{
	QSize size = event->size();
	resizeVideoWidget(size);

	playList->resize(playListWidth, size.height());
	playList->move(size.width() - playListWidth, 0);
	buttonArea->resize(size.width(), buttonAreaHeight);
	buttonArea->move(0, size.height() - buttonAreaHeight);

	QWidget::resizeEvent(event);
}

void VMainWindow::resizeVideoWidget(QSize parentSize)
{
	// Set border to 10px
	QSize size(parentSize.width() - 20, parentSize.height() - 20);
	videoWidget->resize(size);
	videoWidget->move(10, 10);
}

void VMainWindow::mouseMoveEvent(QMouseEvent* event)
{
	logger.logDebug("Mouse moved.");
	int pos_x = event->x();
	int pos_y = event->y();
	if (pos_x > videoWidget->width() + 10)
	{
		playList->show();
	}
	else if (pos_y > videoWidget->height() + 10)
	{
		buttonArea->show();
	}
	QWidget::mouseMoveEvent(event);
}

void VMainWindow::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	QPixmap pixmap("./Resources/mainBackground.png");
	painter.drawPixmap(0, 0, width(), height(), pixmap);
	QWidget::paintEvent(event);
}

void VMainWindow::hidePlayList()
{
	if (!playList->isContextMenuOpen())
	{
		playList->hide();
	}
}

void VMainWindow::hideButtonArea()
{
	buttonArea->hide();
}