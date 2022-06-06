#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <string>
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <thread>
#include <ctime>
#include <mutex>
#include <condition_variable>
#include <QLabel>

extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"
};
using namespace std;

struct IMAGE_FRAME
{
    QImage image;
    double video_clock;
    int64_t pts;
    AVRational frameRate;

};


/*The controller is used to control the decode and render thraed*/
class Controller
{
public:
    mutex Q_mutex;  //to protect the following Q; 用于保护队列
    std::queue<vector<IMAGE_FRAME>> Q; // store the frames for reverse playing; 存储倒放帧的队列


    mutex seekMutex; // to protect isDecoderSeek,isDisplayerSeek and seekPos
    bool isDecoderSeek = false;
    bool isDisplayerSeek = false;
    qint64 seekPos;


    mutex quitMutex; //to protectisQuit;  用于保护isQuit
    bool isQuit = false;
    mutex mutex_; // to protect the following condition_variable  保护条件变量
    // 满和空的条件变量
    condition_variable cvfull;
    condition_variable cvempt;

    void clear();   // pop all the elements in Q
    void init(); // 重新初始化这个controller的部件内容
    void print();
};

class utils
{
public:
    utils();
};

#endif // UTILS_H
