#ifndef JVP_PlayerList_H
#define JVP_PlayerList_H

#include <QTreeWidget>

class VPlayList :public QTreeWidget
{
	Q_OBJECT
public:
	VPlayList(QWidget* parent = nullptr);
};

#endif // !JVP_PlayerList_H