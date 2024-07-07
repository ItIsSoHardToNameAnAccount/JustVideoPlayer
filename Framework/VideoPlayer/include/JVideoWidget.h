#ifndef JVP_VideoWidget_H
#define JVP_VideoWidget_H

#include <QWidget>

class JVideoWidget :public QWidget
{
	Q_OBJECT
public:
	JVideoWidget(QWidget* parent = nullptr);
protected:
	void mouseDoubleClickEvent(QMouseEvent* event) override;
signals:
	void onWidgetDoubleClicked();
};

#endif // !JVP_VideoWidget_H