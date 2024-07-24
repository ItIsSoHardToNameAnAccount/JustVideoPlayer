#ifndef JVP_ButtonArea_H
#define JVP_ButtonArea_H

#include <QWidget>
#include <QBoxLayout>
#include <QLabel>
#include <QSlider>
#include <QPushButton>

class VButtonArea :public QWidget
{
	Q_OBJECT
public:
	VButtonArea(QWidget* parent = nullptr);
public slots:
	void setVolumeSlider(int value);
	void togglePlayPause();
protected:
	void leaveEvent(QEvent* event) override;
	void paintEvent(QPaintEvent* event) override;
private slots:
	void setVolume(int value);
private:
	QHBoxLayout* mainLayout;

	QWidget* volumeArea;
	QHBoxLayout* volumeLayout;
	QLabel* volumeLabel;
	QSlider* volumeSlider;

	QPushButton* videoPlayerControlButton;

	QPushButton* fullScreenButton;
signals:
	void tipVolume();
	void fullScreenPressed();
	void onMouseLeave();
};

#endif // !JVP_ButtonArea_H