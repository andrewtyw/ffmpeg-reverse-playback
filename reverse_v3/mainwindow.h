#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "reversedisplay.h"
#include "reversedecode.h"
#include <QMutex>
#include <QFileDialog>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public slots:
    void showRatio(qint64);
    void killReverseThread();
    void playVideo();
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    qint64 duration;
    Controller *ctrl = NULL;
    ReverseDisplay *reverseDisplayer = NULL;
    ReverseDecode *reverseDecoder = NULL;
    bool isKilled = false;
    bool isPause = false;
    bool isVideoLoaded=false;
    QString currentVideoPath;

private:
    Ui::MainWindow *ui;

};



#endif // MAINWINDOW_H
