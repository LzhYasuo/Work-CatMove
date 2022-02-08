#ifndef PUBLIC_FUNCTION_H
#define PUBLIC_FUNCTION_H

#include <QWidget>
#include <QCoreApplication>


class QLabel;
class QPushButton;
class QLineEdit;
class QTableView;
class QProgressBar;
class QSlider;
class QRadioButton;
class Public_Function : public QWidget
{
    Q_OBJECT

public:
    Public_Function(QWidget *parent = 0);
    ~Public_Function();

    static char convertCharToHex(char ch);
    //Char 转16进制
    static void convertStringToHex(const QString &str, QByteArray &byteData);
    //QString 转16进制
    static QString appName();
    //返回程序当前所在路径
    static QString appPath();
    //初始化数据库
    static void initDb(const QString &dbName);
    //初始化文件,不存在则拷贝
    static void initFile(const QString &sourceName, const QString &targetName);
    //写入消息到额外的的消息日志文件
    static void writeInfo(const QString &info, bool needWrite = false, const QString &filePath = "log",const QString &FileName = "");
    static void writeError(const QString &info, bool needWrite = false, const QString &filePath = "log",const QString &FileName = "");

    static QString decoderQRCode(QImage img,int width,int height);
};

#endif // PUBLIC_FUNCTION_H
