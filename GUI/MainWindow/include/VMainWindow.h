#ifndef JVP_MainWindow_H
#define JVP_MainWindow_H

#include <QScreen>
#include <QMediaPlayer>
#include <QFrame>
#include <QPushButton>
#include <QLabel>
#include <QSlider>

#include <vlc/vlc.h>

#include "JVideoPlayerBase.h"
#include "JVideoWindow.h"

enum Direction
{
	forward,
	backward
};

class VMainWindow :public JVideoPlayerBase
{
	Q_OBJECT
public:
	VMainWindow(QWidget* parent = nullptr);
	void setVideoWindow(JVideoWindow* videoWindow);
public slots:
	void setNormalScreen();
protected:
	void keyPressEvent(QKeyEvent* event) override;
	void setPlayList() override;
	void playVideoHandler(const char* filePath) override;
private:
	void setWindowToCentral();
	void setVideoWidget();
	void setButtonArea();
	void seek(Direction direction);
	void setVolumeSlider(int value);
	void showVolumeTip(int value);
	void toggleVideoFullScreen();

	QScreen* screen;
	QVBoxLayout* mainLayout;

	QFrame* mainContent;
	QHBoxLayout* topLayout;
	JVideoWidget* videoWidget;

	QFrame* buttonArea;
	QHBoxLayout* buttonAreaLayout;
	QPushButton* videoPlayerControlButton;
	QWidget* volumeArea;
	QHBoxLayout* volumeLayout;
	QLabel* volumeLabel;
	QSlider* volumeSlider;

	QLabel* volumeTip;
	QTimer* volumeTimer;

	bool isFullScreen = false;
private slots:
	void showContextMenu(const QPoint& pos);
	void addVideo();
	void removeVideo(QTreeWidgetItem* item);
	void togglePlayPause();
	void setVolume(int value);
	void setFullScreen();
};

#endif // !JVP_MainWindow_H