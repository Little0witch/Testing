#include "classinformationsensor.h"

classInformationSensor::classInformationSensor() {
    this->time = "00:00:00";  // Время по умолчанию
    this->nameSensor = "UnnamedSensor";  // Имя сенсора по умолчанию
    this->windSpeed = 0;  // Скорость ветра по умолчанию
    this->windDirection = 0;  // Направление ветра по умолчанию
}

classInformationSensor::classInformationSensor(QString time, QString nameSensor, int windSpeed, int windDirection) {
    this->time = time;
    this->nameSensor = nameSensor;
    this->windSpeed = windSpeed;
    this->windDirection = windDirection;
}

QString classInformationSensor::getTime(){
    return time;
}

QString classInformationSensor::getNameSensor(){
    return nameSensor;
}
int classInformationSensor::getWindSpeed(){
    return windSpeed;
}
int classInformationSensor::getWindDirection(){
    return windDirection;
}

void classInformationSensor::setTime(QString time){
    this->time = time;
}

void classInformationSensor::setNameSensor(QString name){
    this->nameSensor = name;
}

void classInformationSensor::setWindSpeed(int speed){
    this->windSpeed = speed;
}

void classInformationSensor::setWindDirection(int direction){
    this->windDirection = direction;
}

void classInformationSensor::setInfrmation(QString time, QString name, int speed, int direction){
    setTime(time);
    setNameSensor(name);
    setWindSpeed(speed);
    setWindDirection(direction);
}

void classInformationSensor::showClass(){
    QString output;
    output = "Time: " + time + ",nameSensor: " + nameSensor
             + ",Speed: " + QString::number(windSpeed) + ",Direction: " + QString::number(windDirection) + ";";
    qDebug() << output;
}
