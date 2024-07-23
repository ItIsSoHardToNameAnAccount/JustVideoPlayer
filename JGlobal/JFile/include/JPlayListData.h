#ifndef JVP_PlayListData_H
#define JVP_PlayListData_H

#include <QJsonArray>

#include "VPlayList.h"

class JPlayListData
{
public:
	static void init();
	static void sync(VPlayList* playList);
	static void load(VPlayList* playList);
	static void save();
private:
	static QJsonArray jsonArray;
};

#endif // !JVP_PlayListData_H