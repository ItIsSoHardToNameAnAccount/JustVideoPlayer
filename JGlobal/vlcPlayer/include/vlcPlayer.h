#ifndef JVP_vlcPlayer_H
#define JVP_vlcPlayer_H

#include <vlc/vlc.h>
#include <QTreeWidget>

enum MediaState
{
	Playing,
	Paused,
	None
};

class MediaData
{
public:
	MediaData(QTreeWidgetItem* item);

	int time = 0;
};

class vlcPlayer
{
public:
	vlcPlayer();
	~vlcPlayer();
	int play(const char* filePath, MediaData mediaData, void* drawable);
	MediaState togglePlayPause();
	void seekForward(int forwardTime);
	void setVolume(int value);
	int getDuration();
	int getCurrentTime();
	void setVideoPosition(int position);
	MediaState checkVideoPlayPause();
private:
	bool setMedia(const char* path);
	bool setOutputWindow(void* drawable);

	libvlc_instance_t* libvlcInstance;
	libvlc_media_player_t* libvlcMediaPlayer;
};

extern vlcPlayer player;

#endif // !JVP_vlcPlayer_H