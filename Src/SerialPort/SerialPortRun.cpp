#include "SerialPortRun.h"
#include "PortReadThread.h"
#include "PortWriteThread.h"
#include "SerialPort.h"
#include <QThread>
#include <QDebug>

//#define ISTHREAD

void SerialPortRun::Init()
{
#ifdef ISTHREAD
    SerialPort* portObj = new SerialPort();
    QThread* portThread = new QThread();
    portObj->moveToThread(portThread);
    this->connect(portObj,&SerialPort::ThreadStop,portThread,&QThread::quit);
    this->connect(portThread,&QThread::finished,portObj,&SerialPort::deleteLater);
    this->connect(portThread,&QThread::finished,portThread,&QThread::deleteLater);
    this->connect(portThread,&QThread::started,portObj,&SerialPort::Run);
    portThread->start();
#else
    SerialPort* portObj = new SerialPort();
#endif

    //读取线程初始化
    readObj = new PortReadThread();
    readObj->setObjectName("readObj");
    QThread* readThread = new QThread();
    readObj->moveToThread(readThread);

    this->connect(readThread,&QThread::started,readObj,&PortReadThread::Run);
    this->connect(readObj,&PortReadThread::ThreadStop,readThread,&QThread::quit);
    this->connect(readThread,&QThread::finished,readObj,&PortReadThread::deleteLater);
    this->connect(readThread,&QThread::finished,readThread,&QThread::deleteLater);
    this->connect(portObj,&SerialPort::SendReadData,this,[=](QString str)
    {
       _mux.lock();
       readObj->AddStr(str);
       _mux.unlock();
    });

    //发送线程初始化
    writeObj = new PortWriteThread();
    writeObj->setObjectName("writeobj");
    QThread* writeThread = new QThread();
    writeObj->moveToThread(writeThread);

    this->connect(writeThread,&QThread::started,writeObj,&PortWriteThread::Run);
    this->connect(writeObj,&PortWriteThread::ThreadStop,writeThread,&QThread::quit);
    this->connect(writeThread,&QThread::finished,writeObj,&PortWriteThread::deleteLater);
    this->connect(writeThread,&QThread::finished,writeThread,&QThread::deleteLater);
    this->connect(writeObj,&PortWriteThread::SendWriteData,portObj,&SerialPort::readyWriteSlot);


    //退出释放内存
    this->connect(this,&SerialPortRun::CloseAll,this,[=](){
        readObj->CloseThread();
        writeObj->CloseThread();
#ifdef ISTHREAD
        portObj->CloseThread();
#else
        portObj->deleteLater();
#endif
    });

    writeThread->start();
    readThread->start();
}

void SerialPortRun::Close()
{
    emit CloseAll();
}


void SerialPortRun::WritePort(QString msg)
{
    _mux.lock();
    writeObj->AddStr(msg);
    _mux.unlock();
}

SerialPortRun::SerialPortRun(QObject *obj)
{

}

SerialPortRun::~SerialPortRun()
{

}
