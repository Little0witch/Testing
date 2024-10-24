#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initComPort(); // инициализация портов
    getPathToSaveJSON(); // получение пути для сохранения JSON
    ui->textBrowser->append(pathToSaveJSON);
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

// получение списка всех портов
QList<QSerialPortInfo> MainWindow::getAvailablePorts(){
    return QSerialPortInfo::availablePorts();
}

//проверка на измнение портов
bool MainWindow::isChangeAvailableListPorts(){
    QList<QSerialPortInfo> tmpList = getAvailablePorts();
    // изменение размерности списка
    if (tmpList.size() != listAvailableComPort.size()) {
        return true;
    }
    // проверка на изменение элементов списка
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
        QMessageBox::critical(this, "Error", "Нет доступных com-портов. Программа будет закрыта.");
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
        QList<qint32> listBaundRatesComPort = getBaudRatesComPort();
        for(qint32 baundRate : listBaundRatesComPort){
            ui->comboBoxSpeed->addItem(QString::number(baundRate));
        }

        // изначально выбирается первый доступный порт
        for(const QSerialPortInfo& port : listAvailableComPort){
            serialPort.setPortName(port.portName());
            if (serialPort.open(QIODevice::ReadWrite))
                break;
        }
        // настройка порта
        serialPort.setBaudRate(QSerialPort::Baud2400);
        serialPort.setDataBits(QSerialPort::Data8);
        serialPort.setStopBits(QSerialPort::OneStop);
        serialPort.setParity(QSerialPort::NoParity);
        serialPort.setFlowControl(QSerialPort::NoFlowControl);
        // установка скорости по-умолчанию в comboBox
        ui->comboBoxSpeed->setCurrentText(QString::number(serialPort.baudRate()));
    }
}

// обновление выпадающего спсика портов
void MainWindow::updateComPorts(){
    if (isChangeAvailableListPorts()){
        listAvailableComPort.clear(); // очистка списка
        listAvailableComPort = getAvailablePorts(); // информация о доступных портах
        if (listAvailableComPort.empty()){
            QMessageBox::critical(this, "Error", "Нет доступных com-портов. Программа будет закрыта.");
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

// получение пути для сохранения файла
void MainWindow::getPathToSaveJSON()
{
    QDir dir;
    QString pathProg = dir.absolutePath(); // путь к запускающемуся файлу
    int index = pathProg.indexOf("test1");
    pathToSaveJSON = pathProg.mid(0,index) + "sensor_data.json";
}

// просто удобный список с возможными скоростями
QList<qint32> MainWindow::getBaudRatesComPort(){

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

// проверка на корректность данных с помощью регулярного выражения
bool MainWindow::isCorrectData(const QString& data)
{
    QRegularExpression regex(R"(\$[0-9]+\.[0-9]{2},[0-9]{3}\.[0-9]{2})");
    QRegularExpressionMatch match = regex.match(data);
    if (match.hasMatch()){
        return true;
    }
    else{
        return false;
    }
}

// чтение данных
void MainWindow::readData()
{
    QByteArray data = serialPort.readAll();
    QString message = QString::fromUtf8(data);
    ui->textBrowserASCII->insertPlainText(message);
    // Прокрутка вниз для отображения новых данных
    ui->textBrowserASCII->moveCursor(QTextCursor::End);
    if (isCorrectData(data)){
        classInformationSensor informationSensor = translateASCII(message);
        showResult(informationSensor);
        writeJSON(informationSensor);
    }
    else{
        ui->textBrowserResult->append("Некорректные данные.");
    }

}

void MainWindow::showResult(const classInformationSensor& data)
{
    QString time = "Время: " + data.getTime().toString("hh:mm:ss");
    QString windSpeed = "Скорсть ветра (среднее значение): " + QString::number(data.getWindSpeed()) + " м/с";
    QString windDirection = "Направление ветра (среднее значение): " + QString::number(data.getWindDirection()) + "°" ;
    ui->textBrowserResult->append(time);
    ui->textBrowserResult->append(windSpeed);
    ui->textBrowserResult->append(windDirection);
    ui->textBrowserResult->append("");
}

// запись в JSON
void MainWindow::writeJSON(const classInformationSensor& data)
{
    QJsonObject newEntry;
    newEntry["Time"] = data.getTime().toString(Qt::ISODate);
    newEntry["name_sensor"] = data.getNameSensor();
    newEntry["speed"] = QString::number(data.getWindSpeed(), 'f', 2);
    newEntry["direction"] = QString::number(data.getWindDirection(), 'f', 2);

    QFile file(pathToSaveJSON);
    if (file.open(QIODevice::ReadOnly)) {
        QByteArray fileData = file.readAll();
        file.close();
        // Чтение существующего массива JSON
        QJsonDocument doc = QJsonDocument::fromJson(fileData);
        if (!doc.isNull() && doc.isArray()) {
            jsonArray = doc.array();
        }
    }

    // Добавление нового объекта в массив
    jsonArray.append(newEntry);

    // Запись обновленного массива в файл
    if (file.open(QIODevice::WriteOnly)) {
        QJsonDocument saveDoc(jsonArray);
        file.write(saveDoc.toJson());
        file.close();
    }
}

// перевод из ASCII в понятный человеку
classInformationSensor MainWindow::translateASCII(QString data)
{
    classInformationSensor informationSensor;
    QString nameSensor = "WMT700";
    if (isCorrectData(data)){
        data = data.mid(1,(data.length()-3));
        QStringList listData = data.split(",");
        informationSensor.setInformation(QDateTime::currentDateTime(), nameSensor, listData.first().toFloat(), listData.last().toFloat());
        informationSensor.classToString();
    }
    else{
        QMessageBox::information(this,"Information", "Некорректные данные.");
    }
    return informationSensor;
}

// заполнение выпадающего бокса
void MainWindow::on_comboBoxSpeed_activated(int index)
{
    // установка скорости порта
    serialPort.setBaudRate(getBaudRatesComPort()[index]);
}

// изменение выпадающего списка портов
void MainWindow::on_comboBoxComPorts_activated(int index)
{
    // установка нового порта
    QString newNamePort = ui->comboBoxComPorts->currentText();
    if (serialPort.portName() != newNamePort)
    {
        QSerialPort tmpPort;
        tmpPort.setPortName(newNamePort);
        if (!tmpPort.open(QIODevice::ReadWrite)){
            QMessageBox::critical(this,"Error", "Этот порт занят!");
            ui->comboBoxComPorts->setCurrentText(serialPort.portName());
            tmpPort.close();
        }
        else {
            serialPort.close();
            tmpPort.close();
            serialPort.setPortName(newNamePort);
            serialPort.open(QIODevice::ReadWrite);
        }
    }
}

