#ifndef _SERIALPORT_RUN_H_
#define _SERIALPORT_RUN_H_

#include <QObject>
#include <QMutex>

class PortWriteThread;
class PortReadThread;
class SerialPortRun : public QObject
{
    Q_OBJECT
public:
    static SerialPortRun* Get()
    {
        static SerialPortRun cx;
        return &cx;
    }

    void Init();
    void Close();
    void WritePort(QString msg);

signals:
    void CloseAll();
private:
    SerialPortRun(QObject* obj = nullptr);
    ~SerialPortRun();
private:
    QMutex _mux;
    PortWriteThread* writeObj;
    PortReadThread* readObj;
};

#endif //_SERIALPORT_RUN_H_
