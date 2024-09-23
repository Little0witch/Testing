#ifndef CLASSINFORMATIONSENSOR_H
#define CLASSINFORMATIONSENSOR_H
#include <QDateTime>

class classInformationSensor
{
private:
    QDateTime time;
    QString nameSensor;
    float windSpeed;
    float windDirection;

public:
    classInformationSensor();
    classInformationSensor(const QDateTime& time, const QString& name, float speed, float direction);
    QDateTime getTime() const;
    QString getNameSensor() const;
    float getWindSpeed() const;
    float getWindDirection() const;
    void setTime(const QDateTime& time);
    void setNameSensor(const QString& name);
    void setWindSpeed(float speed);
    void setWindDirection(float direction);
    QString classToString() const;
    void setInformation(const QDateTime& time, const QString& name, float speed, float direction);

};

#endif // CLASSINFORMATIONSENSOR_H
