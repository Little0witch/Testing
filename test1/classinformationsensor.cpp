#include "classinformationsensor.h"

classInformationSensor::classInformationSensor() {
    this->time = "00:00:00";  // Время по умолчанию
    this->nameSensor = "UnnamedSensor";  // Имя сенсора по умолчанию
    this->windSpeed = 0;  // Скорость ветра по умолчанию
    this->windDirection = 0;  // Направление ветра по умолчанию
}

classInformationSensor::classInformationSensor(QString time, QString nameSensor, float windSpeed, float windDirection) {
    this->time = time;
    this->nameSensor = nameSensor;
    this->windSpeed = windSpeed;
    this->windDirection = windDirection;
}

QString classInformationSensor::getTime() const{
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

void classInformationSensor::setTime(QString time){
    this->time = time;
}

void classInformationSensor::setNameSensor(QString name){
    this->nameSensor = name;
}

void classInformationSensor::setWindSpeed(float speed){
    this->windSpeed = speed;
}

void classInformationSensor::setWindDirection(float direction){
    this->windDirection = direction;
}

void classInformationSensor::setInfrmation(QString time, QString name, float speed, float direction){
    setTime(time);
    setNameSensor(name);
    setWindSpeed(speed);
    setWindDirection(direction);
}

QString classInformationSensor::classToString(){
    return QString("Time: %1, Sensor: %2, Wind Speed: %3, Wind Direction: %4")
        .arg(time)
        .arg(nameSensor)
        .arg(QString::number(windSpeed, 'f', 2))
        .arg(QString::number(windDirection, 'f', 2));
}
