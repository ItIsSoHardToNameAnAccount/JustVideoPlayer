#include "JVolume.h"

void JVolume::init()
{
	volumeValue = 100;
}

void JVolume::setVolume(int value)
{
	volumeValue = value;
}

int JVolume::getVolume()
{
	return volumeValue;
}