#include "Public_Function.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QTime>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QDateTime>
#include <QLabel>
#include <QPushButton>
#include <QPainter>
#include <QTranslator>
#include <QTextCodec>
#include <QThread>
#include <QProcess>
#include <QSettings>
#include <QFileDialog>
#include <QTimer>
#include <QTcpSocket>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkInterface>
#include <QHostInfo>
#include <QTableView>
#include <QHeaderView>
#include <QMessageBox>
#include <QLineEdit>
#include <QProgressBar>
#include <QRadioButton>
#include <QPainter>
#include "QZXing.h"


const QString ERROR = "error";

Public_Function::Public_Function(QWidget *parent)
    : QWidget(parent)
{
}

Public_Function::~Public_Function()
{

}

char Public_Function::convertCharToHex(char ch)
{
    /*
    0x30等于十进制的48，48也是0的ASCII值，，
    1-9的ASCII值是49-57，，所以某一个值－0x30，，
    就是将字符0-9转换为0-9

    */
    if((ch >= '0') && (ch <= '9'))
        return ch-0x30;
    else if((ch >= 'A') && (ch <= 'F'))
        return ch-'A'+10;
    else if((ch >= 'a') && (ch <= 'f'))
        return ch-'a'+10;
    else return (-1);
}

void Public_Function::convertStringToHex(const QString &str, QByteArray &byteData)
{
    int hexdata,lowhexdata;
    int hexdatalen = 0;
    int len = str.length();
    byteData.resize(len/2);
    char lstr,hstr;
    for(int i=0; i<len; )
    {
        //char lstr,
        hstr=str[i].toLatin1();
        if(hstr == ' ')
        {
            i++;
            continue;
        }
        i++;
        if(i >= len)
            break;
        lstr = str[i].toLatin1();
        hexdata = convertCharToHex(hstr);
        lowhexdata = convertCharToHex(lstr);
        if((hexdata == 16) || (lowhexdata == 16))
            break;
        else
            hexdata = hexdata*16+lowhexdata;
        i++;
        byteData[hexdatalen] = (char)hexdata;
        hexdatalen++;
    }
    byteData.resize(hexdatalen);
}

//返回程序文件名称
QString Public_Function::appName()
{
    static QString name;
    if(name.isEmpty())
    {
        name = qApp->applicationName();
        QStringList list = name.split("/");
        name = list.at(list.count() - 1).split(".").at(0);
    }
    return name;
}

//返回程序当前所在路径
QString Public_Function::appPath()
{
    //如果定义在Android下面
#ifdef Q_OS_ANDROID
    return QString("/sdcard/Android/%1").arg(appName());
#else
    return qApp->applicationDirPath();
#endif
}

//写入消息到额外的的消息日志文件
void Public_Function::writeInfo(const QString &info, bool needWrite, const QString &filePath,const QString &FileName)
{
    if(!needWrite)
        return;

    QString fileName = QString("%1/%2%3_%4reinfo.txt")
            .arg(Public_Function::appPath()).arg(filePath).arg(Public_Function::appName()).arg(FileName)/*.arg(QDate::currentDate().toString())*/;

    QFile File(fileName);
    if(File.open(QIODevice::WriteOnly | QIODevice::Append | QFile::Text))
    {
        QTextStream stream(&File);
        stream << "\n" << QDateTime::currentDateTime().toString() << " " << info;
        File.close();
    }

}
void Public_Function::writeError(const QString &info, bool needWrite, const QString &filePath,const QString &FileName)
{
    if(!needWrite)
        return;
    QString fileName = QString("%1/%2%3_%4reerror.txt")
            .arg(Public_Function::appPath()).arg(filePath).arg(Public_Function::appName()).arg(FileName)/*.arg(QDate::currentDate().toString())*/;

    QFile File(fileName);
    if(File.open(QIODevice::WriteOnly | QIODevice::Append | QFile::Text))
    {
        QTextStream stream(&File);
        stream << "\n" << QDateTime::currentDateTime().toString() << " " << info;
        File.close();
    }
}

QString Public_Function::decoderQRCode(QImage img, int width, int height)
{

    QString msg = "";
    const QImage decoderimg = img;
    if(img.isNull())
    {
        return ERROR;
    }
    img = img.scaled(width,height,Qt::KeepAspectRatio);
    //ui->label->setPixmap(QPixmap::fromImage(img));

    QZXing decoder;
    //解析CODE128一维码和二维码
    decoder.setDecoder(QZXing::DecoderFormat_CODE_128 | QZXing::DecoderFormat_QR_CODE);
    QString result = decoder.decodeImage(img);
    if(result.isEmpty())
    {
        msg = ERROR;
    }

    return msg;
}
