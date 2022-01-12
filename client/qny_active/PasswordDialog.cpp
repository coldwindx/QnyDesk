#include "PasswordDialog.h"
#include "ui_PasswordDialog.h"

PasswordDialog::PasswordDialog(QWidget *parent) :
    MoveDialog(parent),
    ui(new Ui::PasswordDialog)
{
    ui->setupUi(this);

    connect(ui->connectBtn, &QPushButton::clicked, this, &PasswordDialog::connectEvent);
}

PasswordDialog::~PasswordDialog()
{
    delete ui;
}

void PasswordDialog::connectEvent()
{
    QString password = ui->passwordEdit->text().remove(QRegExp("\\s"));
    emit link(password);
}
