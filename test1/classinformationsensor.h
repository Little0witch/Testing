#ifndef CLASSINFORMATIONSENSOR_H
#define CLASSINFORMATIONSENSOR_H
#include <QDateTime>

class classInformationSensor
{
private:
    QString time;
    QString nameSensor;
    float windSpeed;
    float windDirection;

public:
    classInformationSensor();
    classInformationSensor(QString, QString, float, float);
    QString getTime() const;
    QString getNameSensor() const;
    float getWindSpeed() const;
    float getWindDirection() const;
    void setTime(QString);
    void setNameSensor(QString);
    void setWindSpeed(float);
    void setWindDirection(float);
    QString classToString();
    void setInfrmation(QString, QString, float, float);

};

#endif // CLASSINFORMATIONSENSOR_H
