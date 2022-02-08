#include "PortReadThread.h"
#include <QDebug>
#include <QThread>

PortReadThread::PortReadThread()
{

}

PortReadThread::~PortReadThread()
{

}

//开启循环
void PortReadThread::Run()
{
    //qDebug() << "PortReadThread" << QThread::currentThreadId();
    _timer = new QTimer(this);
    this->connect(_timer,&QTimer::timeout,this,&PortReadThread::RunThread);
    _timer->start(10000);
}

//线程循环 根据定时器时间循环 最好设置成40毫秒
void PortReadThread::RunThread()
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
        QString str = _list.takeFirst();
//        JudgeTenSion(str);
    }
    _mux.unlock();
}

//退出线程
void PortReadThread::CloseThread()
{
    _mux.lock();
    _isClose = true;
    _mux.unlock();
}

//判断是否是张力值
void PortReadThread::JudgeTenSion(QString str)
{
    if(str.size() < 22)
        return;
    QString leftStr = str.left(12);
    if(leftStr == "100819100a0a")
    {
        int sum = 0;
        for(int i = 2; i <= 18; i+= 2)
        {
            bool ok;
            sum += str.mid(i,2).toInt(&ok,16);
        }
        const QString sumStr = QString::number(sum,16);
        const QString CheckStr = str.right(2);
        if(sumStr != CheckStr)
        {
            return;
        }
        QString midStr =  QString::number(str.mid(12,2).toInt());
        QString mid1Str = QString::number(str.mid(14,2).toInt());
        QString mid2Str = QString::number(str.mid(16,2).toInt());
        QString TenSion = midStr + mid1Str + "." + mid2Str;
        qDebug() << TenSion;
        emit IsTenSion(TenSion);
    }
}

//添加字符串至列表
void PortReadThread::AddStr(QString str)
{
    _mux.lock();
    if(str.isEmpty() || str == "")
    {
        _mux.unlock();
        return;
    }
//    _list.push_back(str);

    _mux.unlock();
}
