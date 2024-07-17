#include "vlcPlayer.h"
#include "JVolume.h"

#include <thread>

vlcPlayer player;

MediaData::MediaData(QTreeWidgetItem* item)
{
	time = item->data(0, Qt::UserRole + 1).toInt();
}

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

int vlcPlayer::play(const char* filePath, MediaData mediaData, void* drawable)
{
	libvlc_time_t lastVideoTime = -1;
	if (libvlc_media_player_is_playing(libvlcMediaPlayer))
	{
		lastVideoTime = libvlc_media_player_get_time(libvlcMediaPlayer);
	}
	if (!setMedia(filePath))return -1;
	if (!setOutputWindow(drawable))return -1;
	if (mediaData.time)
	{
		libvlc_media_player_set_time(libvlcMediaPlayer, mediaData.time);
	}
	libvlc_audio_set_volume(libvlcMediaPlayer, JVolume::getVolume());
	return lastVideoTime;
}

MediaState vlcPlayer::togglePlayPause()
{
	if (!libvlcMediaPlayer)
	{
		return MediaState::None;
	}
	
	libvlc_state_t state = libvlc_media_player_get_state(libvlcMediaPlayer);
	if (state == libvlc_Playing)
	{
		libvlc_media_player_pause(libvlcMediaPlayer);
		return MediaState::Paused;
	}
	else if (state == libvlc_Paused)
	{
		libvlc_media_player_play(libvlcMediaPlayer);
		return MediaState::Playing;
	}
}

void vlcPlayer::seekForward(int forwardTime)
{
	if (!libvlcMediaPlayer)
	{
		return;
	}
	libvlc_time_t currentTime = libvlc_media_player_get_time(libvlcMediaPlayer);
	libvlc_media_player_set_time(libvlcMediaPlayer, currentTime + forwardTime);
}

void vlcPlayer::setVolume(int value)
{
	if (!libvlcMediaPlayer)
	{
		return;
	}
	libvlc_audio_set_volume(libvlcMediaPlayer, value);
}

void vlcPlayer::switchOutputWindow(void* drawable)
{
	libvlc_time_t currentTime = libvlc_media_player_get_time(libvlcMediaPlayer);
	libvlc_state_t currentState = libvlc_media_player_get_state(libvlcMediaPlayer);

	libvlc_media_player_stop(libvlcMediaPlayer);
	if (setOutputWindow(drawable))
	{
		if (libvlc_media_player_play(libvlcMediaPlayer) == -1)return;
		libvlc_media_player_set_time(libvlcMediaPlayer, currentTime);
		if (currentState == libvlc_Paused)
		{
			pauseVideo();
		}
	}
	else
	{
		logger.logError("Failed to switch window.");
	}
}

void vlcPlayer::pauseVideo()
{
	std::thread t([=]() {
		bool isPlaying = false;
		while (!isPlaying)
		{
			isPlaying = libvlc_media_player_is_playing(libvlcMediaPlayer);
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		libvlc_media_player_pause(libvlcMediaPlayer);
		});
	t.detach();
}