#ifndef _SLAMTEC_RADAR_H_
#define _SLAMTEC_RADAR_H_

#include <QObject>

namespace rp
{
    namespace standalone
    {
        namespace rplidar
        {
            class RPlidarDriver;
        }
    }
}

class SlamtecRadar : public QObject
{
    Q_OBJECT
public:
    static SlamtecRadar* Get();
    //设置usb跟波特率初始化雷达
    void SetUsbDevice(QString usbName,int baudrate);
    //去读/dev底下usb信息
    void SetUsbList();
    //读取_usbList;
    QStringList GetUsbList();
    //连接usb跟波特率
    bool Connect(const QString usbName,int baudrate);
    //读取雷达数据
    void ReadData();
    //转换雷达数据
    QPointF MakePoint(float angle,float dist);
    //关闭
    void Close();

signals:
    void UpdataSlamtecData(float);
private:
    SlamtecRadar(QObject* parent = nullptr);
    ~SlamtecRadar();

    rp::standalone::rplidar::RPlidarDriver *_lidarDriver;
    QStringList _usbList;
    QList<QPointF> _points;
    bool _isClose = false;
    bool _isRun = false;
};

#endif //_SLAMTEC_RADAR_H_
