#ifndef JVP_VideoPlayerBase_H
#define JVP_VideoPlayerBase_H

#include <QWidget>
#include <QBoxLayout>
#include <QTreeWidget>
#include <QLabel>

#include "JVideoWidget.h"
#include "JPlayListData.h"
#include "vlcPlayer.h"
#include "JVolume.h"

class JVideoPlayerBase : public QWidget
{
	Q_OBJECT
public:
	JVideoPlayerBase(QWidget* parent = nullptr);
protected:
	virtual void setPlayList() = 0;
	virtual int playVideoHandler(const char* filePath, QTreeWidgetItem* item) = 0;
	virtual MediaState togglePlayPause();
	void keyPressEvent(QKeyEvent* event) override;
	virtual void setVolumeTipComponent(QLabel* volumeTip, QTimer* volumeTimer);
	virtual void tipCurrentVolume(int currentVolume) = 0;
	
	const int playListMaxWidth = 400;
	const int volumeTipConstantTime = 1000;
protected slots:
	void playVideo(QTreeWidgetItem* item, int column);
	virtual void setVolume(int value) = 0;
private:
	void seekForward(int forwardTime);
	void updateLastItem(int time);

	const int baseForwardTime = 5000; // 5s
	const int baseVolumeChangeValue = 10;

	QTreeWidgetItem* lastItem = nullptr;
};

#endif // !JVP_VideoPlayerBase_H