#ifndef JVP_MainWindow_H
#define JVP_MainWindow_H

#include <QScreen>
#include <QMediaPlayer>
#include <QFrame>
#include <QPushButton>
#include <QSlider>
#include <QTreeWidget>
#include <QBoxLayout>
#include <QLabel>

#include <vlc/vlc.h>

#include "vlcPlayer.h"
#include "VPlayList.h"

class VMainWindow :public QWidget
{
	Q_OBJECT
public:
	VMainWindow(QWidget* parent = nullptr);
protected:
	void keyPressEvent(QKeyEvent* event) override;
	void focusOutEvent(QFocusEvent* event) override;
	void resizeEvent(QResizeEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void paintEvent(QPaintEvent* event) override;
private:
	void setWindowToCentral();
	void setVideoWidget();
	void setPlayList();
	int playVideoHandler(const char* filePath, QTreeWidgetItem* item);
	void updateLastItem(int time);
	void setButtonArea();
	void tipCurrentVolume(int currentVolume);
	void togglePlayPauseHandler();
	void setVolumeTipComponent();
	void seekForward(int forwardTime);
	void setVolumeSlider(int value);
	void resizeVideoWidget(QSize parentSize);

	QScreen* screen;

	QWidget* videoWidget;
	VPlayList* playList;

	QFrame* buttonArea;
	QHBoxLayout* buttonAreaLayout;
	QWidget* volumeArea;
	QHBoxLayout* volumeLayout;
	QLabel* volumeLabel;
	QSlider* volumeSlider;
	QLabel* volumeTip;
	QTimer* volumeTimer;
	QPushButton* videoPlayerControlButton;
	QPushButton* fullScreenButton;

	bool isFullScreen = false;

	QTreeWidgetItem* lastItem = nullptr;
private slots:
	void showContextMenu(const QPoint& pos);
	void addVideo();
	void removeVideo(QTreeWidgetItem* item);
	void playVideo(QTreeWidgetItem* item, int column);
	void setVolume(int value);
	void togglePlayPause();
	void setFocusToWindow();
	void setFullScreen();
	void hidePlayList();
};

#endif // !JVP_MainWindow_H