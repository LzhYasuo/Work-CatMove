#include "Pub_Function.h"
#include "QZXing.h"

const QString ERROR = "error";

Pub_Function *Pub_Function::Get()
{
    static Pub_Function cx;
    return &cx;
}

QString Pub_Function::decoderQRCode(QImage img,int width, int height)
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

Pub_Function::Pub_Function()
{

}

Pub_Function::~Pub_Function()
{

}
