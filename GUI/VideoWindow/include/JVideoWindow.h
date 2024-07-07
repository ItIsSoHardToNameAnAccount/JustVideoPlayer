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
protected:
	void setPlayList() override;
	void playVideoHandler(const char* filePath) override;
	void mouseDoubleClickEvent(QMouseEvent* event) override;
	bool eventFilter(QObject* watched, QEvent* event) override;
private:
	void setNormalScreen();
signals:
	void onWidgetDoubleClicked();
};

#endif // !JVP_VideoWindow_H