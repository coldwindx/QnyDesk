#ifndef PENCIL_BUTTON_H_
#define PENCIL_BUTTON_H_

#include <QPushButton>
#include <QMenu>
#include <QFile>

class PencilButton : public QPushButton
{
    Q_OBJECT
public:
    explicit PencilButton(QWidget *parent = nullptr) : QPushButton(parent)
    {
        menu = new QMenu();
        // <1> 刷新密码
        QAction * reflash = new QAction("刷新密码", this);
        connect(reflash, &QAction::triggered, this, [&](){ emit this->reflash(); });
        menu->addAction(reflash);
        // <2> 设置密码
//        QAction * replace = new QAction("设置密码", this);
//        connect(replace, &QAction::triggered, this, [&](){ emit this->replace(); });
//        menu->addAction(replace);
        // <3> 加载样式表
        QFile file("://css/menu.css");
        if(file.open(QFile::ReadOnly))
        {
            menu->setStyleSheet(file.readAll());
            file.close();
        }
        this->setMenu(menu);
    }
    ~PencilButton()
    {
        delete menu;
    }

signals:
    void reflash();
    void replace();
private:
    QMenu * menu;
};

#endif // PASSWORDBUTTON_H
