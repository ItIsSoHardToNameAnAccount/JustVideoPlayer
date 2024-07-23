#ifndef JVP_PlayList_H
#define JVP_PlayList_H

#include <QTreeWidget>

class VPlayList :public QTreeWidget
{
	Q_OBJECT
public:
	VPlayList(QWidget* parent = nullptr);
	void toggleContextMenu(bool state);
	bool isContextMenuOpen();
protected:
	void leaveEvent(QEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
private:
	bool contextMenu = false;
signals:
	void onMouseLeave();
};

#endif // !JVP_PlayList_H