#include "PortWriteThread.h"
#include "../Pub/Public_Function.h"
#include <QDebug>
#include <QThread>

PortWriteThread::PortWriteThread()
{

}

PortWriteThread::~PortWriteThread()
{

}

void PortWriteThread::Run()
{
    //qDebug() << "PortWriteThread" << QThread::currentThreadId();
    _timer = new QTimer(this);
    this->connect(_timer,&QTimer::timeout,this,&PortWriteThread::RunThead);
    _timer->start(40);
}


void PortWriteThread::RunThead()
{
    _mux.lock();
    if(_isClose)
    {
        _timer->stop();
        _timer->deleteLater();
        _mux.unlock();
        emit ThreadStop();
        return;
    }
    if(_list.size() == 0)
    {
        _mux.unlock();
        return;
    }
    while(_list.size() > 0)
    {
        QString msg = _list.takeFirst();
        QByteArray ary;
        Public_Function::convertStringToHex(msg,ary);
//        qDebug() << msg << " : ary :" << ary;
        emit SendWriteData(ary);
    }
    _mux.unlock();
}

void PortWriteThread::CloseThread()
{
    _mux.lock();
    _isClose = true;
    _mux.unlock();
}

void PortWriteThread::AddStr(QString str)
{
    _mux.lock();
    if(str.isEmpty() && str == "")
    {
        _mux.unlock();
        return;
    }
    _list.push_back(str);
    _mux.unlock();

}
