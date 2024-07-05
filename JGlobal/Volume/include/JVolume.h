#ifndef JVP_Volume_H
#define JVP_Volume_H

class JVolume
{
public:
	static void init();
	static void setVolume(int value);
	static int getVolume();
private:
	static int volumeValue;
};

#endif // !JVP_Volume_H