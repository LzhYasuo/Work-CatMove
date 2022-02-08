#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QWidget>
#include <QSerialPort>
#include <QSerialPortInfo>

#define ISNotTHREAD

//因为这个类不会频繁发送 所以大概率不会卡死主线程 若是大量发送卡死主线程 请把这个类转为线程使用
#ifdef ISNotTHREAD
class SerialPort : public QSerialPort
{
    Q_OBJECT
public:
    SerialPort(QObject* parent = nullptr);
    ~SerialPort();

public slots:
    void readyWriteSlot(QByteArray msg);
private slots:
    void readyReadSlot();
signals:
    //将接受的信号发送出去
    void SendReadData(QString);
private:
};
#else

class SerialPort : public QObject
{
    Q_OBJECT
public:
    SerialPort(QObject* parent = nullptr);
    ~SerialPort();
    void CloseThread();

    void Run();

public slots:
    void readyWriteSlot(QString msg);
private slots:
    void readyReadSlot();
signals:
    void SendReadData(QString);
    void ThreadStop();

private:
    QSerialPort* _port;
};

#endif

#endif // SERIALPORT_H
