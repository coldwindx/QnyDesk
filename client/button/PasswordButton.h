#ifndef PASSWORDBUTTON_H
#define PASSWORDBUTTON_H

#include <QPushButton>
#include <QMenu>
#include <QFile>

class PasswordButton : public QPushButton
{
    Q_OBJECT
public:
    explicit PasswordButton(QWidget *parent = nullptr) : QPushButton(parent)
    {
        menu = new QMenu();
        // <1> 刷新密码
        menu->addAction(new QAction("刷新密码"));
        // <2> 设置密码
        menu->addAction(new QAction("设置密码"));
        // <3> 加载样式表
        QFile file("://css/menu.css");
        if(file.open(QFile::ReadOnly))
        {
            menu->setStyleSheet(file.readAll());
            file.close();
        }
        this->setMenu(menu);
    }
    ~PasswordButton()
    {
        delete menu;
    }
private:
    QMenu * menu;
};

#endif // PASSWORDBUTTON_H