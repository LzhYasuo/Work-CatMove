#include "SlamtecRadar.h"
#include <QDir>
#include <QDebug>
#include <QtConcurrent>


#include <QThread>
#include <math.h>
#include "rplidar.h"
#include "rplidar_driver.h"

//static rp::standalone::rplidar::RPlidarDriver  *_lidarDriver;

//using namespace rp::standalone::rplidar;


#define _countof(_Array) (sizeof(_Array) / sizeof(_Array[0]))

const int BARCOUNT = 75;
const float ANGLESCALE = 360.0/BARCOUNT;
const int DISTSCALE = 10000;

SlamtecRadar *SlamtecRadar::Get()
{
    static SlamtecRadar cx;
    return &cx;
}

void SlamtecRadar::SetUsbDevice(QString usbName,int baudrate)
{
   _lidarDriver = rp::standalone::rplidar::RPlidarDriver::CreateDriver(rp::standalone::rplidar::DRIVER_TYPE_SERIALPORT);
   bool isConnect = Connect(usbName,baudrate);
   if(!isConnect)
   {
       return;
   }
   _isRun = true;
   QtConcurrent::run(this,&SlamtecRadar::ReadData);
}

void SlamtecRadar::ReadData()
{
//    QMutex mux;
    while(!_isClose)
    {
        float stopAngle = 0;
//        mux.lock();
        rplidar_response_measurement_node_hq_t nodes[8192];
        size_t count = _countof(nodes);

//        qDebug() << "Count : " << count;
//        qDebug() << "waiting for data";

        u_result res = _lidarDriver->grabScanDataHq(nodes,count);
        if(res & RESULT_FAIL_BIT)
        {
            qDebug() << "Error in scanning";
            continue;
        }
        res = _lidarDriver->ascendScanData(nodes,count);
        if(res & RESULT_FAIL_BIT)
        {
            qDebug() << "Error in scanning";
            continue;
        }
        _points.clear();
        for(int pos = 0; pos < static_cast<int>(count); ++pos)
        {
            QPointF f = QPointF(nodes[pos].angle_z_q14 * 90.f / 16384.f,nodes[pos].dist_mm_q2 / 1000.0f / 4.0f);
            _points.push_back(f);
            if(nodes[pos].angle_z_q14 * 90.f / 16384.f > 0 && nodes[pos].angle_z_q14 * 90.f / 16384.f < 1)
            {
                stopAngle = nodes[pos].dist_mm_q2 / 1000.0f / 4.0f;
            }
        }
        emit UpdataSlamtecData(stopAngle);
        QThread::usleep(100);
//        mux.unlock();
    }
}

QPointF SlamtecRadar::MakePoint(float angle,float dist)
{
    float relativeDist = dist / DISTSCALE;
    float x = sin((angle / 360.0 * 2 * M_PI) * relativeDist);
    float y = cos((angle / 360.0 * 2 * M_PI) * relativeDist);

    return QPointF(x,y);
}

void SlamtecRadar::Close()
{
    if(_isRun)
    {
        _isClose = true;
        _lidarDriver->stopMotor();
        _lidarDriver->disconnect();
        rp::standalone::rplidar::RPlidarDriver::DisposeDriver(_lidarDriver);
    }
}

void SlamtecRadar::SetUsbList()
{
    QDir devDir("/dev");
    QStringList filers;
    filers << "*USB*";

    QFileInfoList infoList = devDir.entryInfoList(filers,QDir::System);
    _usbList.clear();
    for(auto &i : infoList)
    {
        _usbList.push_back(i.absoluteFilePath());
    }
}

QStringList SlamtecRadar::GetUsbList()
{
    return _usbList;
}

bool SlamtecRadar::Connect(const QString usbName, int baudrate)
{
    bool isConnect = true;
    rplidar_response_device_info_t devinfo;
    u_result op_result;
    op_result = _lidarDriver->connect(usbName.toUtf8(),baudrate);
    if(op_result & RESULT_FAIL_BIT)
    {
        qDebug() << "Failed to connect to " << usbName << " : " << op_result;
        isConnect = false;
        return isConnect;
    }
    op_result = _lidarDriver->getDeviceInfo(devinfo);
    if(op_result & RESULT_FAIL_BIT)
    {
        qDebug() << "Failed to connect to " << usbName << " : " << op_result;
        isConnect = false;
        return isConnect;
    }
    QString serialNum;
    for(int i = 0; i < 16; i++)
    {
        serialNum.append(devinfo.serialnum[i]);
    }
    qDebug() << "Connect to " << serialNum;
    //开启旋转马达
    _lidarDriver->startMotor();
    //RPLIDAR 扫描模式
    op_result = _lidarDriver->startScan(0,1);
    if(op_result & RESULT_FAIL_BIT)
    {
        qDebug() << "Unable to Start scan";
        isConnect = false;

    }
    return isConnect;
}

SlamtecRadar::SlamtecRadar(QObject *parent)
    :QObject (parent)
{

}

SlamtecRadar::~SlamtecRadar()
{

}
