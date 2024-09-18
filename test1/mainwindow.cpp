#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initComPort();
}

MainWindow::~MainWindow()
{
    // всё, что открыто, должно быть закрыто
    if(serialPort.isOpen())
        serialPort.close();
    delete ui;
}


void MainWindow::initComPort()
{
    listAvailableComPort.clear(); // очистка списка
    listAvailableComPort = QSerialPortInfo::availablePorts(); // информация о доступных портах
    // проверка наличия com-портов
    if (listAvailableComPort.empty()){
        QMessageBox::critical(this, "Error", "No com ports available. The program will be closed");
        exit(3); // вывод сообщения и закрытие программы
    }
    else{
        ui->comboBoxComPorts->clear(); // очистка бокса
    for(QSerialPortInfo itemComPort : listAvailableComPort){
        // заполнение выпадающего списка доступными портами
        ui->comboBoxComPorts->addItem(itemComPort.portName());
    }
    }
}



void MainWindow::on_comboBoxComPorts_currentIndexChanged(int index)
{
// добавить сохранение и подключение ком-порта
    qDebug() << listAvailableComPort[index].portName();
}

