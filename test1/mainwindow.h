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
    void initComPort();
    QList<qint32> getBaundRatesComPort();
    void readData();
    void showDataASCII(QString);
    void writeJSON(QString);
    void translateASCII(QString);

private slots:
    void on_comboBoxComPorts_currentIndexChanged(int index);
    void on_comboBoxSpeed_activated(int index);

private:
    Ui::MainWindow *ui;
    QList<QSerialPortInfo> listAvailableComPort; // списк всех доступных портов
    QSerialPort serialPort; // выбранный порт
    QFile file;
};
#endif // MAINWINDOW_H
