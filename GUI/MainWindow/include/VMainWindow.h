#ifndef JVP_MainWindow_H
#define JVP_MainWindow_H

#include <QWidget>
#include <QScreen>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QBoxLayout>
#include <QFrame>
#include <QPushButton>
#include <vlc/vlc.h>

#include "VPlayList.h"

enum Direction
{
	forward,
	backward
};

class VMainWindow :public QWidget
{
	Q_OBJECT
public:
	VMainWindow(QWidget* parent = nullptr);
	~VMainWindow();
protected:
	void keyPressEvent(QKeyEvent* event) override;
private:
	void setWindowToCentral();
	void setVideoPlayer();
	void setPlayList();
	void setButtonArea();
	void startVideo(const QString& filePath);
	void seek(Direction direction);

	QScreen* screen;

	/*
	Main layout contains two area, bottom area is the button area.
	Top area contains video player and player list.
	*/
	QVBoxLayout* mainLayout;

	QFrame* mainContent;
	QHBoxLayout* topLayout;
	QWidget* videoWidget;
	libvlc_instance_t* libvlcInstance;
	libvlc_media_player_t* libvlcMediaPlayer;
	VPlayList* playList;

	QFrame* buttonArea;
	QHBoxLayout* buttonAreaLayout;
	QPushButton* videoPlayerControlButton;
private slots:
	void showContextMenu(const QPoint& pos);
	void addVideo();
	void removeVideo(QTreeWidgetItem* item);
	void playVideo(QTreeWidgetItem* item, int column);
	void togglePlayPause();
};

#endif // !JVP_MainWindow_H