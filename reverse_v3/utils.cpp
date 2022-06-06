#include "utils.h"
#include <QDebug>
void Controller::clear(){
    while (!Q.empty()) Q.pop();
}

void Controller::init(){
    clear();
    isDecoderSeek = false;
    isDisplayerSeek = false;
    isQuit = false;
}

void Controller::print(){
    qDebug()<<"----------------";
    qDebug()<<"isDecoderSeek"<< isDecoderSeek;
    qDebug()<<"isDisplayerSeek"<< isDisplayerSeek;
    qDebug()<<"isQuit"<< isQuit;
    qDebug()<<"Q.size"<< Q.size();
    qDebug()<<"----------------";
}
