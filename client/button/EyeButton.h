#ifndef EYE_BUTTON_H_
#define EYE_BUTTON_H_

#include <QPushButton>
#include <QMenu>
#include <QFile>

class EyeButton : public QPushButton
{
    Q_OBJECT
public:
    explicit EyeButton(QWidget *parent = nullptr) : QPushButton(parent) {}
    ~EyeButton() {}
    bool changeState()
    {
        return state = !state;
    }

    bool getState()
    {
        return state;
    }
private:
    bool state = true;
};

#endif // PASSWORDBUTTON_H
