#include "JPlayListData.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

const char* playListDataFile = "playlistData.json";

QJsonArray JPlayListData::jsonArray;

void JPlayListData::init()
{
	QFile file(playListDataFile);
	if (!file.open(QIODevice::ReadOnly))
	{
		logger.logError("Failed to load play list.");
		return;
	}

	QByteArray data = file.readAll();
	file.close();

	QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
	jsonArray = jsonDoc.array();
}

void JPlayListData::sync(VPlayList* playList)
{
	QJsonArray dataArray;
	for (int i = 0; i < playList->topLevelItemCount(); ++i)
	{
		QTreeWidgetItem* item = playList->topLevelItem(i);
		saveItem(item, dataArray);
	}
	jsonArray = dataArray;
}

void JPlayListData::load(VPlayList* playList)
{
	playList->clear();
	for (const QJsonValue& value : jsonArray)
	{
		QJsonObject jsonObject = value.toObject();
		QTreeWidgetItem* item = new QTreeWidgetItem(playList);
		item->setText(0, jsonObject["text"].toString());
		item->setData(0, Qt::UserRole, jsonObject["path"].toString());
		item->setData(0, Qt::UserRole + 1, jsonObject["time"].toInt());
		item->setData(0, Qt::UserRole + 2, jsonObject["type"].toInt());

		QJsonArray childrenArray = jsonObject["children"].toArray();
		for (const QJsonValue& childValue : childrenArray)
		{
			QJsonObject childObject = childValue.toObject();
			QTreeWidgetItem* childItem = loadItem(childObject, item);
			item->addChild(childItem);
		}
	}
}

void JPlayListData::save()
{
	QJsonDocument jsonDoc(jsonArray);
	QFile file(playListDataFile);
	if (file.open(QIODevice::WriteOnly))
	{
		file.write(jsonDoc.toJson());
		file.close();
	}
	else
	{
		logger.logError("Failed to save play list.");
	}
}

void JPlayListData::saveItem(QTreeWidgetItem* item, QJsonArray& dataArray)
{
	QJsonObject jsonObject;
	jsonObject["text"] = item->text(0);
	jsonObject["path"] = item->data(0, Qt::UserRole).toString();
	jsonObject["time"] = item->data(0, Qt::UserRole + 1).toInt();
	jsonObject["type"] = item->data(0, Qt::UserRole + 2).toInt();

	QJsonArray childrenArray;
	for (int i = 0; i < item->childCount(); ++i)
	{
		saveItem(item->child(i), childrenArray);
	}
	jsonObject["children"] = childrenArray;
	
	dataArray.append(jsonObject);
}

QTreeWidgetItem* JPlayListData::loadItem(const QJsonObject& jsonObject, QTreeWidgetItem* parent)
{
	QTreeWidgetItem* item = new QTreeWidgetItem(parent);
	item->setText(0, jsonObject["text"].toString());
	item->setData(0, Qt::UserRole, jsonObject["path"].toString());
	item->setData(0, Qt::UserRole + 1, jsonObject["time"].toInt());
	item->setData(0, Qt::UserRole + 2, jsonObject["type"].toInt());

	QJsonArray childrenArray = jsonObject["children"].toArray();
	for (const QJsonValue& value : childrenArray)
	{
		QJsonObject childObject = value.toObject();
		QTreeWidgetItem* childItem = loadItem(childObject, item);
		item->addChild(childItem);
	}

	return item;
}