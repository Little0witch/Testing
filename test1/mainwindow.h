#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QJsonObject>
#include <QFile>
#include <QJsonDocument>
#include <QTimer>
#include <classinformationsensor.h>
#include <QDir>
#include <QJsonArray>
#include <QRegularExpression>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QList<QSerialPortInfo> getAvailablePorts();
    bool isChangeAvailableListPorts();
    void initComPort();
    void updateComPorts();
    void getPathToSaveJSON();
    QList<qint32> getBaudRatesComPort();
    bool isCorrectData(const QString& data);
    void readData();
    void showDataASCII(QString);
    void writeJSON(const classInformationSensor&);
    classInformationSensor translateASCII(QString);
    void showResult(const classInformationSensor& data);

private slots:
    void on_comboBoxSpeed_activated(int index);
    void on_comboBoxComPorts_activated(int index);

private:
    Ui::MainWindow *ui;
    QList<QSerialPortInfo> listAvailableComPort; // списк всех доступных портов
    QSerialPort serialPort; // выбранный порт
    QString pathToSaveJSON; // путь к файлу
    QJsonArray jsonArray; // массив данных JSON
};
#endif // MAINWINDOW_H
