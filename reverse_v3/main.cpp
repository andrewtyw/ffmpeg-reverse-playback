#include "mainwindow.h"

#include <QApplication>
#include <QDebug>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
#include "include/SDL.h"
}

#undef main
int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    qDebug()<<av_version_info();
    return a.exec();
}
