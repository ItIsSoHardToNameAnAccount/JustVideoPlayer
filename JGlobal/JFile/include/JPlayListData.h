#ifndef JVP_PlayListData_H
#define JVP_PlayListData_H

#include <QJsonArray>

#include "VPlayList.h"

enum JPlayListItemType
{
	Video,
	Folder
};

class JPlayListData
{
public:
	static void init();
	static void sync(VPlayList* playList);
	static void load(VPlayList* playList);
	static void save();
private:
	static QJsonArray jsonArray;

	static void saveItem(QTreeWidgetItem* item, QJsonArray& dataArray);
	static QTreeWidgetItem* loadItem(const QJsonObject& jsonObject, QTreeWidgetItem* parent = nullptr);
};

#endif // !JVP_PlayListData_H