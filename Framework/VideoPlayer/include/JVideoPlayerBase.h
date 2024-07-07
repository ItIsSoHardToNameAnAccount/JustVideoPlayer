#ifndef JVP_VideoPlayerBase_H
#define JVP_VideoPlayerBase_H

#include <QWidget>
#include <QBoxLayout>
#include <QTreeWidget>

#include "JVideoWidget.h"
#include "JPlayListData.h"

class JVideoPlayerBase : public QWidget
{
	Q_OBJECT
public:
	JVideoPlayerBase(QWidget* parent = nullptr);
protected:
	virtual void setPlayList() = 0;
	virtual void playVideoHandler(const char* filePath) = 0;

	QTreeWidget* playList;
	
	const int playListMaxWidth = 400;
protected slots:
	void playVideo(QTreeWidgetItem* item, int column);
};

#endif // !JVP_VideoPlayerBase_H