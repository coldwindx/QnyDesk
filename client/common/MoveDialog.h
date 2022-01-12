#ifndef MOVEDIALOG_H
#define MOVEDIALOG_H

#include <QDialog>
#include <QMouseEvent>

class MoveDialog : public QDialog
{
    Q_OBJECT
public:
    explicit MoveDialog(QWidget * parent = nullptr) : QDialog(parent)
    {
        setWindowFlags(Qt::FramelessWindowHint |
                       Qt::Tool | Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint);
    }
    void mousePressEvent(QMouseEvent *e) override
    {
        last = e->globalPos();
        press = e->pos();
    }
    void mouseMoveEvent(QMouseEvent *e) override
    {
        if(titleHeight < press.y())
            return ;
        int dx = e->globalX() - last.x();
        int dy = e->globalY() - last.y();
        last =e->globalPos();
        move(x() + dx, y() + dy);
    }
    void mouseReleaseEvent(QMouseEvent *e) override
    {
        if(titleHeight < press.y())
            return ;
        int dx = e->globalX() - last.x();
        int dy = e->globalY() - last.y();
        move(x() + dx, y() + dy);
    }

    void pressedAdjust()
    {
        press.setX(100);
        press.setY(100);
    }
    void setTopHeight(int height)
    {
        this->titleHeight = height;
    }
private:
    QPoint last;
    QPoint press;
    int titleHeight;
};

#endif // MOVEDIALOG_H
