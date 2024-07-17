#include <QApplication>

#include "VMainWindow.h"
#include "JVolume.h"
#include "JPlayListData.h"
#include "JVideoWindow.h"

void init()
{
	JVolume::init();
	JPlayListData::init();
}

int main(int argc, char** argv)
{
	init();

	QApplication JustVideoPlayer(argc, argv);

	VMainWindow mainWindow;

	return JustVideoPlayer.exec();
}