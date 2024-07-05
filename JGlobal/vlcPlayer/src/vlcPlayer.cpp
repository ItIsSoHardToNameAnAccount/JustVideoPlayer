#include "vlcPlayer.h"
#include "JVolume.h"

vlcPlayer player;

vlcPlayer::vlcPlayer()
{
	libvlcInstance = libvlc_new(0, nullptr);
	libvlcMediaPlayer = libvlc_media_player_new(libvlcInstance);
}

vlcPlayer::~vlcPlayer()
{
	if (libvlcMediaPlayer)
	{
		libvlc_media_player_stop(libvlcMediaPlayer);
		libvlc_media_player_release(libvlcMediaPlayer);
	}
	if (libvlcInstance)
	{
		libvlc_release(libvlcInstance);
	}
}

bool vlcPlayer::setMedia(const char* path)
{
	libvlc_media_t* media = libvlc_media_new_path(libvlcInstance, path);
	if (!media)
	{
		logger.logDebug("libvlc_media_new_path failed!");
		return false;
	}

	libvlc_media_player_set_media(libvlcMediaPlayer, media);
	libvlc_media_release(media);

	return true;
}

bool vlcPlayer::setOutputWindow(void* drawable)
{
	libvlc_media_player_set_hwnd(libvlcMediaPlayer, drawable);
	if (libvlc_media_player_play(libvlcMediaPlayer) == -1)
	{
		logger.logDebug("libvlc_media_player_play failed!");
		return false;
	}

	return true;
}

bool vlcPlayer::play(const char* path, void* drawable)
{
	if (!setMedia(path))return false;
	if (!setOutputWindow(drawable))return false;
	libvlc_audio_set_volume(libvlcMediaPlayer, JVolume::getVolume());
	return true;
}