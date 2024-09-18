#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initComPort();

    connect(&serialPort, &QSerialPort::readyRead, this, &MainWindow::readData);

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
        QMessageBox::critical(this, "Error", "No com ports available. The program will be closed.");
        exit(3); // вывод сообщения и закрытие программы
    }
    else{
        ui->comboBoxComPorts->clear(); // очистка бокса
        ui->comboBoxSpeed->clear();
        for(const QSerialPortInfo& itemComPort : listAvailableComPort){
        // заполнение выпадающего списка доступными портами
            ui->comboBoxComPorts->addItem(itemComPort.portName());
        }


        // занесение в боксы скорости
        QList<qint32> listBaundRatesComPort = getBaundRatesComPort();
        for(qint32 baundRate : listBaundRatesComPort){
            ui->comboBoxSpeed->addItem(QString::number(baundRate));
        }

        // изначально выбирается первый доступный порт
        for(const QSerialPortInfo& port : listAvailableComPort){
            serialPort.setPortName(port.portName());
            if (serialPort.open(QIODevice::ReadWrite))
                break;
        }
        serialPort.setBaudRate(QSerialPort::Baud9600);
        serialPort.setDataBits(QSerialPort::Data8);
        serialPort.setStopBits(QSerialPort::OneStop);
        serialPort.setParity(QSerialPort::NoParity);
        serialPort.setFlowControl(QSerialPort::NoFlowControl);
        // установка скорости по-умолчанию в comboBox
        ui->comboBoxSpeed->setCurrentText(QString::number(serialPort.baudRate()));
    }
}

// просто удобный список с возможными скоростями
QList<qint32> MainWindow::getBaundRatesComPort(){

    QList<qint32> baudRates;
    baudRates << QSerialPort::Baud1200
              << QSerialPort::Baud2400
              << QSerialPort::Baud4800
              << QSerialPort::Baud9600
              << QSerialPort::Baud19200
              << QSerialPort::Baud38400
              << QSerialPort::Baud57600
              << QSerialPort::Baud115200;
    return baudRates;

}

void MainWindow::readData()
{
    QByteArray data = serialPort.readAll();
    QString message = QString::fromUtf8(data);
    qDebug() << message;
    // ui->textBrowserASCII->append(message);
    // Вывод в QTextBrowser без добавления лишних переводов строк
    ui->textBrowserASCII->insertPlainText(message);

    // Прокрутка вниз для отображения новых данных
    ui->textBrowserASCII->moveCursor(QTextCursor::End);
}

void MainWindow::on_comboBoxComPorts_currentIndexChanged(int index)
{
    // установка нового порта
    serialPort.setPort(listAvailableComPort[index]);
}

void MainWindow::on_comboBoxSpeed_activated(int index)
{
    // установка скорости порта
    serialPort.setBaudRate(getBaundRatesComPort()[index]);
}

