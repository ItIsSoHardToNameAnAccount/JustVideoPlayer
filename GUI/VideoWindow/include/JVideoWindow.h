#ifndef JVP_VideoWindow_H
#define JVP_VideoWindow_H

#include "JVideoPlayerBase.h"

class JVideoWindow :public JVideoPlayerBase
{
	Q_OBJECT
public:
	JVideoWindow(QWidget* parent = nullptr);
public slots:
	void setFullScreen();
	void closeHiddenWindow();
protected:
	void setPlayList() override;
	void playVideoHandler(const char* filePath) override;
	bool eventFilter(QObject* watched, QEvent* event) override;
	void tipCurrentVolume(int currentVolume) override;
	void keyPressEvent(QKeyEvent* event) override;
protected slots:
	void setVolume(int value) override;
private:
	void setNormalScreen();

	QTreeWidget* playList;
	QLabel* volumeTip;
	QTimer* volumeTimer;
private slots:
	void reciveDoubleClickSignal();
signals:
	void onWidgetDoubleClicked();
};

#endif // !JVP_VideoWindow_H