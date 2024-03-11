#ifndef VIDEOWINDOW_H
#define VIDEOWINDOW_H

#include <QDialog>
#include <QOpenGLWidget>

namespace Ui {
	class VideoWindow;
}

class VideoWindow : public QDialog
{
	Q_OBJECT

public:
	explicit VideoWindow(QWidget *parent = nullptr);
	~VideoWindow();

private:
	Ui::VideoWindow *ui;
	QOpenGLWidget* openGLWidget;
};

#endif // VIDEOWINDOW_H