#ifndef SETTING_BUTTON_H_
#define SETTING_BUTTON_H_

#include <QWidget>
#include <QPushButton>
#include <QMenu>
#include <QAction>
#include <QFile>

class SettingButton : public QPushButton
{
    Q_OBJECT
private:
    QMenu * menu;
public:
    SettingButton(QWidget * parent = nullptr) : QPushButton(parent)
    {
        menu = new QMenu(this);

        QAction * help = new QAction("帮助信息", this);
        QAction * about = new QAction("关于", this);

        menu->addAction(help);
        menu->addAction(about);

        QFile file("://css/menu.css");
        if(file.open(QIODevice::ReadOnly))
        {
            menu->setStyleSheet(file.readAll());
            file.close();
        }
        this->setMenu(menu);
    }
    ~SettingButton() {}
};
#endif