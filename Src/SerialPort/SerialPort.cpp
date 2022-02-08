#include "SerialPort.h"
#include <QDebug>
#include "../Pub/Public_Function.h"
#include <QThread>

#ifdef ISNotTHREAD
SerialPort::SerialPort(QObject *parent)
    : QSerialPort(parent)
{
    this->setPortName("/dev/ttyUSB1");
    this->open(QIODevice::ReadWrite);
    this->setBaudRate(115200);
    this->setDataBits(QSerialPort::Data8);
    this->setParity(QSerialPort::NoParity);
    this->setStopBits(QSerialPort::OneStop);
    this->setFlowControl(QSerialPort::NoFlowControl);

    this->connect(this,&SerialPort::readyRead,this,&SerialPort::readyReadSlot);
}

void SerialPort::readyReadSlot()
{
    if(this->bytesAvailable() > 0)
    {
        QByteArray byes = this->readAll();
        if(!byes.isEmpty())
        {
            //发送
            QByteArray hexData = byes.toHex();
//            qDebug() << hexData;
            waitForReadyRead(10);
//            qDebug() << hexData;
            emit SendReadData(hexData);
        }
    }
}

void SerialPort::readyWriteSlot(QByteArray msg)
{
    if(msg.isEmpty())
    {
        return;
    }
//    qDebug() << msg;
    int datelen = msg.length();
    int mLen = 0;
    while(datelen > 0)
    {
        mLen = (int)this->write(msg,msg.length());
        if(mLen > 0)
        {
            this->waitForBytesWritten();
        }
        if(mLen == -1)
        {
            return;
        }
        datelen -= mLen;
    }
}

SerialPort::~SerialPort()
{

}

#else
SerialPort::SerialPort(QObject* parent)
    :QObject(parent)
{

}

SerialPort::~SerialPort()
{

}

void SerialPort::readyReadSlot()
{
    if(_port->bytesAvailable() > 0)
    {
        QByteArray byes = _port->readAll();
        if(!byes.isEmpty())
        {
            //发送
            QByteArray hexData = byes.toHex();
            _port->waitForReadyRead(100);
            emit SendReadData(hexData);
        }
    }
}

void SerialPort::Run()
{
    qDebug() << "SerialPort" << QThread::currentThreadId();
    _port = new QSerialPort();
    _port->setPortName("com6");
    _port->open(QIODevice::ReadWrite);
    _port->setBaudRate(2400);
    _port->setDataBits(QSerialPort::Data8);
    _port->setParity(QSerialPort::NoParity);
    _port->setStopBits(QSerialPort::OneStop);
    _port->setFlowControl(QSerialPort::NoFlowControl);

    this->connect(_port,&QSerialPort::readyRead,this,&SerialPort::readyReadSlot);
}

void SerialPort::readyWriteSlot(QString msg)
{
    QString cmd = msg;
    if(cmd.isEmpty())
    {
        return;
    }
    int datelen = cmd.length();
    int mLen = 0;
    while(datelen > 0)
    {
        mLen = (int)_port->write(cmd.toLatin1(),cmd.length());
        if(mLen > 0)
        {
            _port->waitForBytesWritten();
        }
        if(mLen == -1)
        {
            return;
        }
        datelen -= mLen;
    }
}

void SerialPort::CloseThread()
{
    _port->deleteLater();
    _port = nullptr;

    emit ThreadStop();
}


#endif
