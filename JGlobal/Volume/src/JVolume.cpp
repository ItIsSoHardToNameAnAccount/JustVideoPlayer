#include "JVolume.h"
#include "vlcPlayer.h"

void JVolume::init()
{
	volumeValue = 100;
}

void JVolume::setVolume(int value)
{
	volumeValue = value;
	player.setVolume(value);
}

int JVolume::getVolume()
{
	return volumeValue;
}