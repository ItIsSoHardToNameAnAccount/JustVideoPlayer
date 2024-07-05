#include "JVideoPlayerBase.h"

JVideoPlayerBase::JVideoPlayerBase(QWidget* parent) :QWidget(parent)
{
	playList = new QTreeWidget(this);
}

void JVideoPlayerBase::playVideo(QTreeWidgetItem* item, int column)
{
	if (item->childCount() == 0)
	{
		QString filePath = item->data(0, Qt::UserRole).toString();
		std::string filePathStr = filePath.toStdString();
		std::replace(filePathStr.begin(), filePathStr.end(), '/', '\\');
		const char* c_filePath = filePathStr.c_str();
		playVideoHandler(c_filePath);
	}
}