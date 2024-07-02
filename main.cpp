#include <QApplication>

#include "VMainWindow.h"

int main(int argc, char** argv)
{
	QApplication JustVideoPlayer(argc, argv);
	VMainWindow mainWindow;
	return JustVideoPlayer.exec();
}