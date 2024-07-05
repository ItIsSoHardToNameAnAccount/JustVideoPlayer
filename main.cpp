#include <QApplication>

#include "VMainWindow.h"
#include "JVolume.h"

void init()
{
	JVolume::init();
}

int main(int argc, char** argv)
{
	init();

	QApplication JustVideoPlayer(argc, argv);
	VMainWindow mainWindow;
	return JustVideoPlayer.exec();
}