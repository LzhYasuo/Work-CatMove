#ifndef _PUB_FUNCTION_H_
#define _PUB_FUNCTION_H_

#include <QObject>

class Pub_Function : public QObject
{
public:
    static Pub_Function* Get();
    QString decoderQRCode(QImage img,int width,int height);

private:
	Pub_Function();
	~Pub_Function();
};


#endif //_PUB_FUNCTION_H_
