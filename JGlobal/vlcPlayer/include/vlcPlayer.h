#ifndef JVP_vlcPlayer_H
#define JVP_vlcPlayer_H

#include <vlc/vlc.h>

class vlcPlayer
{
public:
	vlcPlayer();
	~vlcPlayer();
	bool play(const char* path, void* drawable);
private:
	bool setMedia(const char* path);
	bool setOutputWindow(void* drawable);

	libvlc_instance_t* libvlcInstance;
	libvlc_media_player_t* libvlcMediaPlayer;
};

extern vlcPlayer player;

#endif // !JVP_vlcPlayer_H