#ifndef JVP_VideoWindow_H
#define JVP_VideoWindow_H

#include "JVideoPlayerBase.h"

class JVideoWindow :public JVideoPlayerBase
{
	Q_OBJECT
public:
	JVideoWindow(QWidget* parent = nullptr);
protected:
	void setPlayList() override;
	void playVideoHandler(const char* filePath) override;
};

#endif // !JVP_VideoWindow_H