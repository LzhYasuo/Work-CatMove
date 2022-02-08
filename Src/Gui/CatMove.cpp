#include "CatMove.h"
#include "../Pub/SlamtecRadar.h"
#include "../SerialPort/SerialPortRun.h"
#include "QDebug"
#include <QTimer>
#include <QPushButton>
#include <QHBoxLayout>

const QString PortStr1 = "7B0000001E00000000657D";
const QString PortStr2 = "7B00000000000000007B7D";
const QString PortStr3 = "7B0000FFE200000000667D";

CatMove::CatMove(QWidget *parent) :
    QWidget(parent)
{
    Init();

    SerialPortRun::Get()->Init();

    SlamtecRadar::Get()->SetUsbList();
    QStringList msg = SlamtecRadar::Get()->GetUsbList();
    if(msg.size() <= 0)
        return;

    SlamtecRadar::Get()->SetUsbDevice(msg.at(0),115200);

//    this->connect(SlamtecRadar::Get(),&SlamtecRadar::UpdataSlamtecData,this,[=](float p)
//    {
//        qDebug() << p;
//        if(p != 0.0f && p <= 0.19f)
//        {
//            qDebug () << p;
//            SerialPortRun::Get()->WritePort(PortStr2);
//        }
////        else if(p != 0.0f && p > 0.25f)
////        {
////            qDebug() << p;
////            SerialPortRun::Get()->WritePort(PortStr1);
////        }
//    });

    SerialPortRun::Get()->WritePort(PortStr3);

    mTimer = new QTimer(this);
    this->connect(mTimer,&QTimer::timeout,this,[=](){
        SerialPortRun::Get()->WritePort(PortStr2);
    });
    mTimer->start(500);

}

void CatMove::closeEvent(QCloseEvent *event)
{
    SerialPortRun::Get()->Close();
    SlamtecRadar::Get()->Close();
    this->close();
//    SerialPortRun::Get()->WritePort(PortStr2);
//    SerialPortRun::Get()->Close();
}

void CatMove::Init()
{
    QHBoxLayout* mLayout = new QHBoxLayout();
    mLayout->setSpacing(0);
    mLayout->setContentsMargins(0,0,0,0);

    QPushButton* btn_forward = new QPushButton("forward");
    QPushButton* btn_stop = new QPushButton("stop");
    QPushButton* btn_back = new QPushButton("back");

    mLayout->addWidget(btn_forward);
    mLayout->addWidget(btn_stop);
    mLayout->addWidget(btn_back);

    this->connect(btn_forward,&QPushButton::clicked,this,[=](){
        SerialPortRun::Get()->WritePort(PortStr1);
    });
    this->connect(btn_stop,&QPushButton::clicked,this,[=](){
        SerialPortRun::Get()->WritePort(PortStr2);
    });
    this->connect(btn_back,&QPushButton::clicked,this,[=](){
        SerialPortRun::Get()->WritePort(PortStr3);
    });

    this->setLayout(mLayout);
}

CatMove::~CatMove()
{

}

