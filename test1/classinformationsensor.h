#ifndef CLASSINFORMATIONSENSOR_H
#define CLASSINFORMATIONSENSOR_H
#include <QDateTime>

class classInformationSensor
{
private:
    QString time;
    QString nameSensor;
    qint8 windSpeed;
    qint8 windDirection;

public:
    classInformationSensor();
    classInformationSensor(QString, QString, int, int);
    QString getTime();
    QString getNameSensor();
    int getWindSpeed();
    int getWindDirection();
    void setTime(QString);
    void setNameSensor(QString);
    void setWindSpeed(int);
    void setWindDirection(int);
    void showClass();
    void setInfrmation(QString, QString, int, int);

};

#endif // CLASSINFORMATIONSENSOR_H
