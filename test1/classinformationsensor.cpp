#include "classinformationsensor.h"

classInformationSensor::classInformationSensor() {
    this->time = QDateTime::currentDateTime();  // Время по умолчанию
    this->nameSensor = "UnnamedSensor";  // Имя сенсора по умолчанию
    this->windSpeed = 0;  // Скорость ветра по умолчанию
    this->windDirection = 0;  // Направление ветра по умолчанию
}

classInformationSensor::classInformationSensor(const QDateTime& time, const QString& name, float speed, float direction) {
    this->time = time;
    this->nameSensor = name;
    this->windSpeed = speed;
    this->windDirection = direction;
}

QDateTime classInformationSensor::getTime() const{
    return time;
}

QString classInformationSensor::getNameSensor() const{
    return nameSensor;
}
float classInformationSensor::getWindSpeed() const{
    return windSpeed;
}
float classInformationSensor::getWindDirection() const{
    return windDirection;
}

void classInformationSensor::setTime(const QDateTime& time){
    this->time = time;
}

void classInformationSensor::setNameSensor(const QString& name){
    this->nameSensor = name;
}

void classInformationSensor::setWindSpeed(float speed){
    this->windSpeed = speed;
}

void classInformationSensor::setWindDirection(float direction){
    this->windDirection = direction;
}

void classInformationSensor::setInformation(const QDateTime& time, const QString& name, float speed, float direction){
    setTime(time);
    setNameSensor(name);
    setWindSpeed(speed);
    setWindDirection(direction);
}

QString classInformationSensor::classToString() const{
    return QString("Time: %1, Sensor: %2, Wind Speed: %3, Wind Direction: %4")
        .arg(time.toString("hh:mm:ss"))
        .arg(nameSensor)
        .arg(QString::number(windSpeed, 'f', 2))
        .arg(QString::number(windDirection, 'f', 2));
}
