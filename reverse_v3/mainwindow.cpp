#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utils.h"
#include "reversedisplay.h"
#include "reversedecode.h"
#include <QMessageBox>
#include <QIcon>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // initialize the controller, which contains locks and condition variables
    ctrl = new Controller;
    connect(ui->start,SIGNAL(clicked()),this,SLOT(playVideo()));


    //we can pause the video by locking the rendering thread
    //通过给渲染线程加锁，从而暂停画面的播放*/
    connect(ui->stop_video, &QPushButton::clicked, this,[=](){
        if(reverseDisplayer->pause_==false){
            reverseDisplayer->pauseThread();
            isPause=true;
        }
        else{
            reverseDisplayer->resumeThread();
            isPause=false;
        }
    });

    // seek to specified position
    // 视频时间跳转
    connect(ui->video_slider,&VideoSlider::sig_pressed,this,[=](){
        ctrl->seekMutex.lock();
        ctrl->isDecoderSeek=true;
        ctrl->isDisplayerSeek=true;
        ctrl->seekPos = qint64(ui->video_slider->value());
        ctrl->seekMutex.unlock();

    });

}



















MainWindow::~MainWindow()
{
    delete ui;
}




/*show the progress (pts)*/
/*显示进度（pts）*/
void MainWindow::showRatio(qint64 pts){
    ui->ratio->setText(QString("%1/%2").arg(pts).arg(duration));
    ui->video_slider->setValue(int(pts));
}


/**
 * @brief kill the decode and rendering thread and set them to be NULL before switching to another video.
 *        退出解码和渲染线程（在切换视频的时候需要这么做）
 * @param fileName  文件路径
 * @return no return
 */
void MainWindow::killReverseThread(){
    // use the seek frame lock to lure the thread to quit
    ctrl->seekMutex.lock();
    ctrl->isDisplayerSeek=true;
    ctrl->seekMutex.unlock();
    ctrl->quitMutex.lock();
    ctrl->isQuit=true;
    ctrl->quitMutex.unlock();
    reverseDecoder->exit(0);
    reverseDisplayer->exit(0);
    reverseDecoder->deleteLater();
    reverseDisplayer->deleteLater();
    reverseDecoder->wait();
    reverseDisplayer->wait();
    // disconnect what have been connected
    disconnect(reverseDisplayer, nullptr, ui->widget_video, nullptr);
    disconnect(reverseDisplayer, nullptr, this, nullptr);
    // delete the pointer and set it to NULL
    delete reverseDecoder;
    delete reverseDisplayer;
    reverseDecoder = NULL;
    reverseDisplayer = NULL;
    isKilled=true;

    // re-initialize the controller
    ctrl->init();
    ctrl->print();
    isVideoLoaded=false;
}



void MainWindow::playVideo(){
    // open a file
    QString filename;
    filename = QFileDialog::getOpenFileName();
    if (filename == ""){
        return;
    }
    currentVideoPath = filename;
    // we need to resume the thread if it is pausing
    if(isVideoLoaded&&isPause)
    {
        reverseDisplayer->resumeThread();
        isPause=false;
    }
    // quit the decode and render thread
    if(isVideoLoaded)
        killReverseThread();
    // initialize
    reverseDisplayer = new ReverseDisplay(ctrl,ui->widget_video);
    reverseDecoder = new ReverseDecode(ctrl);

    // initialize the file
    int res = reverseDecoder->loadFile(currentVideoPath);
    if(res!=0){
        QMessageBox::warning(this, tr("Error"),
                                       tr("Something gets wrong")
                                       );
        delete reverseDecoder;
        delete reverseDisplayer;
        reverseDecoder = NULL;
        reverseDisplayer = NULL;
        return;
    }
    duration = reverseDecoder->duration;
    ui->video_slider->setMaximum(duration);
    reverseDecoder->start();
    reverseDisplayer->start();

    // connect signals
    connect(reverseDisplayer, SIGNAL(SendOneFrame(QImage)), ui->widget_video, SLOT(slotSetOneFrame(QImage)));
    connect(reverseDisplayer, SIGNAL(SendTime(qint64)), this, SLOT(showRatio(qint64)));


    isVideoLoaded=true;
}
