#include "reversedisplay.h"

ReverseDisplay::ReverseDisplay(Controller *ctrl,VideoFrameDisplay *videoDisplayer)
{
    this->ctrl = ctrl;
    this->videoDisplayer = videoDisplayer;
    stop_=false;
    pause_=false;
}

void ReverseDisplay::run(){
    while (1) {
        ctrl->quitMutex.lock();
        if(ctrl->isQuit){ctrl->quitMutex.unlock();break;}
        ctrl->quitMutex.unlock();
        vector<IMAGE_FRAME> V;
        {
            unique_lock<mutex> lk(ctrl->mutex_);
            qDebug()<<"DISPLAYER:  waiting for Q to be not empty 正在等待QImage队列非空";
            ctrl->cvfull.wait(lk, [&]() { return ctrl->Q.size() > 0; });
            V = ctrl->Q.front();
            ctrl->Q.pop();
            ctrl->cvempt.notify_one();
        }
        // when reverse playing is ended, the decode will send an empty Vector
        if (V.size() == 0) {
            qDebug()<<"V size=0, quit";
            break;
        }
        for (auto i = V.rbegin(); i != V.rend(); i++) { // 反向迭代

            pauseLock.lock(); //lock for pausing; 暂停锁

            // 查看是否seek
            ctrl->seekMutex.lock();
            if(ctrl->isDisplayerSeek){
                qDebug()<<"DISPLAYER: 检测到了seek";
                // 发现目前正在seek, 立刻退出渲染的循环

                //清空队列Q
                ctrl->Q_mutex.lock();
                ctrl->clear();
                qDebug()<<"DISPLAYER: claer completed,  Qsize:"<<ctrl->Q.size();
                ctrl->Q_mutex.unlock();

                //发出empty的信号(因为Q以及被清空)
                ctrl->cvempt.notify_one(); // 此时Q已经被清空, 需要唤醒正在阻塞(大部分情况下)的decoder线程

                // seek标记返回常态
                ctrl->isDisplayerSeek=false;

                ctrl->seekMutex.unlock();
                pauseLock.unlock();
                break;

            }
            ctrl->seekMutex.unlock();

            SendOneFrame((*i).image);  // 发送帧信号  send frame
            SendTime(qint64((*i).pts)); // 发送时间信号 send the pts of the frame
            pauseLock.unlock();
            int delta = int(1000* 1.0 / (double)av_q2d((*i).frameRate));
            qDebug()<<"DISPLAYER: current pts:"<<(*i).pts<<" time:"<<(*i).video_clock<<"  sleep:"<<delta;

            /*
                delta is the actual time to sleep. But due to the limited render ability of QImage, it would be slow for some
                large videos. Because it needs more extended time to process a QImage for a larger video.
            */
            //QThread::msleep(delta);
            QThread::msleep(40);  // we sleep a fixed time(It should be improved)
        }
    }
    qDebug()<<"DISPLAYER: Quit rendering loop; 退出displayer大循环";
}
void ReverseDisplay::pauseThread(){
    pauseLock.lock();
    pause_=true;
}

void ReverseDisplay::resumeThread(){
    pauseLock.unlock();
    pause_=false;
}


