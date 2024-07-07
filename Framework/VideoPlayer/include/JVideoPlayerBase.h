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
	virtual void playVideoHandler(const char* filePath) = 0;
	virtual MediaState togglePlayPause();
	void keyPressEvent(QKeyEvent* event) override;
	virtual void setVolumeTipComponent(QLabel* volumeTip, QTimer* volumeTimer);
	virtual void setVideoWidgetOverlay() = 0;
	virtual void tipCurrentVolume(int currentVolume) = 0;
	
	const int playListMaxWidth = 400;
	const int volumeTipConstantTime = 1000;
protected slots:
	void playVideo(QTreeWidgetItem* item, int column);
	virtual void setVolume(int value) = 0;
private:
	void seekForward(int forwardTime);

	const int baseForwardTime = 5000; // 5s
	const int baseVolumeChangeValue = 10;
};

#endif // !JVP_VideoPlayerBase_H