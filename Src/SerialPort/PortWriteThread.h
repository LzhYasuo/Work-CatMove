#ifndef _PORT_WRITE_THREAD_H_
#define _PORT_WRITE_THREAD_H_

#include <QObject>
#include <QTimer>
#include <QMutex>

class PortWriteThread : public QObject
{
    Q_OBJECT
public:
    PortWriteThread();
    ~PortWriteThread();

    void CloseThread();

signals:
    void ThreadStop();
    void SendWriteData(QByteArray );

public slots:
    void Run();
    void AddStr(QString);
private:
    void RunThead();
private:
    QTimer* _timer;
    QMutex _mux;
    QList<QString> _list;

    bool _isClose = false;
};

#endif //_PORT_WRITE_THREAD_H_
