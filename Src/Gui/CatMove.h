#ifndef CATMOVE_H
#define CATMOVE_H

#include <QWidget>

class CatMove : public QWidget
{
    Q_OBJECT

public:
    explicit CatMove(QWidget *parent = nullptr);
    ~CatMove();
    void closeEvent(QCloseEvent *event);

private:
    void Init();
private:
    QTimer* mTimer;
};

#endif // CATMOVE_H
