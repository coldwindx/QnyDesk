#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->connectBtn, &QPushButton::clicked, this, &MainWindow::link);

    NetworkHandler * net = new NetworkHandler(this);
    net->createSocket();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::link()
{
    // QMessageBox::information(this, "Information", "This button is clicked!");
    qDebug() << "This button is clicked!";
}