#ifndef JVP_MainWindow_H
#define JVP_MainWindow_H

#include <QScreen>
#include <QMediaPlayer>
#include <QFrame>
#include <QPushButton>
#include <QSlider>
#include <QTreeWidget>

#include <vlc/vlc.h>

class VMainWindow :public QWidget
{
	Q_OBJECT
public:
	VMainWindow(QWidget* parent = nullptr);
public slots:
	void setNormalScreen();
protected:
	void keyPressEvent(QKeyEvent* event) override;
	int playVideoHandler(const char* filePath, QTreeWidgetItem* item) override;
	void closeEvent(QCloseEvent* event) override;
	void tipCurrentVolume(int currentVolume) override;
protected slots:
	void setVolume(int value) override;
private:
	void setWindowToCentral();
	void setPlayList();
	void setButtonArea();
	void setVolumeSlider(int value);

	QScreen* screen;

	QWidget* videoWidget;
	QTreeWidget* playList;

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
private slots:
	void showContextMenu(const QPoint& pos);
	void addVideo();
	void removeVideo(QTreeWidgetItem* item);
	void playVideo(QTreeWidgetItem* item, int column);
	void v_togglePlayPause();
	void setFullScreen();
signals:
	void programClosed();
	void windowHidden();
};

#endif // !JVP_MainWindow_H