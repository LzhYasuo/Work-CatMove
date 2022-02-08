#ifndef _PORT_READ_THREAD_H_
#define _PORT_READ_THREAD_H_

#include <QObject>
#include <QTimer>
#include <QMutex>

class PortReadThread : public QObject
{
    Q_OBJECT
public:
    PortReadThread();
    ~PortReadThread();

    //启动循环
    void Run();
    //添加字符串至列表
    void AddStr(QString str);
    //关闭线程
    void CloseThread();
signals:
    void ThreadStop();
    //是张力值信号
    void IsTenSion(QString);
private:
    void RunThread();
    //判断是否是张力值
    void JudgeTenSion(QString msg);
private:
    QList<QString> _list;
    bool _isClose = false;
    QTimer* _timer;
    QMutex _mux;
};

#endif //_PORT_READ_THREAD_H_
