#ifndef JVP_MainWindow_H
#define JVP_MainWindow_H

#include <QScreen>
#include <QMediaPlayer>
#include <QFrame>
#include <QPushButton>
#include <QSlider>

#include <vlc/vlc.h>

#include "JVideoPlayerBase.h"
#include "JVideoWindow.h"

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
	void closeEvent(QCloseEvent* event) override;
	void tipCurrentVolume(int currentVolume) override;
protected slots:
	void setVolume(int value) override;
private:
	void setWindowToCentral();
	void setVideoWidget();
	void setButtonArea();
	void setVolumeSlider(int value);

	QScreen* screen;
	QVBoxLayout* mainLayout;

	QFrame* mainContent;
	QHBoxLayout* topLayout;
	QWidget* videoWidget;
	QLabel* volumeTip; 
	QTimer* volumeTimer;
	QTreeWidget* playList;

	QFrame* buttonArea;
	QHBoxLayout* buttonAreaLayout;
	QWidget* volumeArea;
	QHBoxLayout* volumeLayout;
	QLabel* volumeLabel;
	QSlider* volumeSlider;
	QPushButton* videoPlayerControlButton;
	QPushButton* fullScreenButton;

	bool isFullScreen = false;
private slots:
	void showContextMenu(const QPoint& pos);
	void addVideo();
	void removeVideo(QTreeWidgetItem* item);
	void v_togglePlayPause();
	void setFullScreen();
signals:
	void programClosed();
};

#endif // !JVP_MainWindow_H