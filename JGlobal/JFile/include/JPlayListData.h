#ifndef JVP_PlayListData_H
#define JVP_PlayListData_H

#include <QJsonArray>
#include <QTreeWidget>

class JPlayListData
{
public:
	static void init();
	static void sync(QTreeWidget* playList);
	static void load(QTreeWidget* playList);
	static void save();
private:
	static QJsonArray jsonArray;
};

#endif // !JVP_PlayListData_H