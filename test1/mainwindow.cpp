#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initComPort(); // инициализация портов
    getPathToSaveJSON(); // получение пути для сохранения JSON
    // связка для работы с com-портом
    connect(&serialPort, &QSerialPort::readyRead, this, &MainWindow::readData);

    // обработка обновления списка портов
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateComPorts);
    timer->start(5000);  // обновление списка каждые 5 секунд
}

MainWindow::~MainWindow()
{
    // всё, что открыто, должно быть закрыто
    if(serialPort.isOpen())
        serialPort.close();
    delete ui;
}

QList<QSerialPortInfo> MainWindow::getAvailablePorts(){
    return QSerialPortInfo::availablePorts();
}

bool MainWindow::isChangeAvailableListPorts(){
    QList<QSerialPortInfo> tmpList = getAvailablePorts();
    if (tmpList.size() != listAvailableComPort.size()) {
        return true;
    }
    for (int i = 0; i < tmpList.size(); ++i) {
        if (tmpList[i].portName() != listAvailableComPort[i].portName()) {
            return true;
        }
    }
    return false;
}

void MainWindow::initComPort()
{
    listAvailableComPort.clear(); // очистка списка
    listAvailableComPort = getAvailablePorts(); // информация о доступных портах
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
        serialPort.setBaudRate(QSerialPort::Baud2400);
        serialPort.setDataBits(QSerialPort::Data8);
        serialPort.setStopBits(QSerialPort::OneStop);
        serialPort.setParity(QSerialPort::NoParity);
        serialPort.setFlowControl(QSerialPort::NoFlowControl);
        // установка скорости по-умолчанию в comboBox
        ui->comboBoxSpeed->setCurrentText(QString::number(serialPort.baudRate()));
    }
}

void MainWindow::updateComPorts(){
    if (isChangeAvailableListPorts()){
        listAvailableComPort.clear(); // очистка списка
        listAvailableComPort = getAvailablePorts(); // информация о доступных портах
        if (listAvailableComPort.empty()){
            QMessageBox::critical(this, "Error", "No com ports available. The program will be closed.");
            exit(3); // вывод сообщения и закрытие программы
        }
        else{
            ui->comboBoxComPorts->clear(); // очистка бокса
            for(const QSerialPortInfo& itemComPort : listAvailableComPort){
                // заполнение выпадающего списка доступными портами
                ui->comboBoxComPorts->addItem(itemComPort.portName());
            }
            ui->comboBoxComPorts->setCurrentText(serialPort.portName());
            // установка скорости по-умолчанию в comboBox
            ui->comboBoxSpeed->setCurrentText(QString::number(serialPort.baudRate()));
        }
    }
}

void MainWindow::getPathToSaveJSON()
{
    QDir dir;
    QString pathProg = dir.absolutePath();
    int index = pathProg.indexOf("test1");
    pathToSaveJSON = pathProg.mid(0,index) + "sensor_data.json";
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

    ui->textBrowserASCII->insertPlainText(message);
    // Прокрутка вниз для отображения новых данных
    ui->textBrowserASCII->moveCursor(QTextCursor::End);

    classInformationSensor informationSensor = translateASCII(message);
    ui->textBrowserResult->append(informationSensor.classToString());
    writeJSON(informationSensor);
}

void MainWindow::writeJSON(const classInformationSensor& data)
{
    QJsonObject newEntry;
    newEntry["time"] = data.getTime();
    newEntry["name_sensor"] = data.getNameSensor();
    newEntry["speed"] = data.getWindSpeed();
    newEntry["direction"] = data.getWindDirection();
    QFile file(pathToSaveJSON);
    if (file.open(QIODevice::ReadOnly)) {
        QByteArray fileData = file.readAll();
        file.close();

        // Читаем существующий массив JSON
        QJsonDocument doc = QJsonDocument::fromJson(fileData);
        if (!doc.isNull() && doc.isArray()) {
            jsonArray = doc.array();
        }
    }

    // Добавляем новый объект в массив
    jsonArray.append(newEntry);

    // Записываем обновленный массив в файл
    if (file.open(QIODevice::WriteOnly)) {
        QJsonDocument saveDoc(jsonArray);
        file.write(saveDoc.toJson());
        file.close();
    }
}


classInformationSensor MainWindow::translateASCII(QString data)
{
    classInformationSensor informationSensor;
    QString time = QTime::currentTime().toString("hh:mm:ss");
    QString nameSensor = "WMT700";
    if (data.startsWith("$") && data.endsWith("\r\n")){
        data = data.mid(1,(data.length()-3));
        QStringList listData = data.split(",");
        informationSensor.setInfrmation(time, nameSensor, listData.first().toFloat(), listData.last().toFloat());
        informationSensor.classToString();
    }
    else{
        QMessageBox::information(this,"Information", "Incorrect data.");
    }
    return informationSensor;
}

void MainWindow::on_comboBoxSpeed_activated(int index)
{
    // установка скорости порта
    serialPort.setBaudRate(getBaundRatesComPort()[index]);
}


void MainWindow::on_comboBoxComPorts_activated(int index)
{
    // установка нового порта
    QString newNamePort = ui->comboBoxComPorts->currentText();
    if (serialPort.portName() != newNamePort)
    {
        QSerialPort tmpPort;
        tmpPort.setPortName(newNamePort);
        if (!tmpPort.open(QIODevice::ReadWrite)){
            QMessageBox::critical(this,"Error", "This port is not available!");
            ui->comboBoxComPorts->setCurrentText(serialPort.portName());
            tmpPort.close();
        }
        else {
            serialPort.close();
            tmpPort.close();
            serialPort.setPortName(newNamePort);
            // serialPort.setPort(listAvailableComPort[index]);
            serialPort.open(QIODevice::ReadWrite);
        }
    }
}

