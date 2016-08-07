#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>

#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/text.hpp>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void SeleccionarVideo();
    void ProcesarVideo(bool checked);

    void groupsDraw(cv::Mat &src, std::vector< cv::Rect> &groups);
private slots:
    void on_toolButton_clicked();

    void on_actionAbrir_Video_triggered();

    void on_play_toggled(bool checked);

private:
    Ui::MainWindow *ui;
    cv::VideoCapture cap;
};

#endif // MAINWINDOW_H

