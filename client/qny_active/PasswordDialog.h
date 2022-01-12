#ifndef PASSWORDDIALOG_H
#define PASSWORDDIALOG_H

#include <QDialog>
#include "common/MoveDialog.h"

namespace Ui {
    class PasswordDialog;
}

class PasswordDialog : public MoveDialog
{
    Q_OBJECT

public:
    explicit PasswordDialog(QWidget *parent = nullptr);
    ~PasswordDialog();
signals:
    void link(QString);
private:
    Ui::PasswordDialog *ui;

    void connectEvent();
};

#endif // PASSWORDDIALOG_H
