#ifndef JVP_vlcPlayer_H
#define JVP_vlcPlayer_H

#include <vlc/vlc.h>

enum MediaState
{
	Playing,
	Paused,
	None
};

class vlcPlayer
{
public:
	vlcPlayer();
	~vlcPlayer();
	bool setMedia(const char* path);
	bool setOutputWindow(void* drawable);
	bool play(const char* path, void* drawable);
	MediaState togglePlayPause();
	void seekForward(int forwardTime);
	void setVolume(int value);
private:

	libvlc_instance_t* libvlcInstance;
	libvlc_media_player_t* libvlcMediaPlayer;
};

extern vlcPlayer player;

#endif // !JVP_vlcPlayer_H