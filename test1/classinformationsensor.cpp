#include "classinformationsensor.h"

// конструктор
classInformationSensor::classInformationSensor() {
    this->time = QDateTime::currentDateTime();  // Время по умолчанию
    this->nameSensor = "UnnamedSensor";  // Имя сенсора по умолчанию
    this->windSpeed = 0;  // Скорость ветра по умолчанию
    this->windDirection = 0;  // Направление ветра по умолчанию
}

// конструктор с параметрами
classInformationSensor::classInformationSensor(const QDateTime& time, const QString& name, float speed, float direction) {
    this->time = time;
    this->nameSensor = name;
    this->windSpeed = speed;
    this->windDirection = direction;
}

// геттер времени
QDateTime classInformationSensor::getTime() const{
    return time;
}

// геттер
QString classInformationSensor::getNameSensor() const{
    return nameSensor;
}

// геттер скорости
float classInformationSensor::getWindSpeed() const{
    return windSpeed;
}

// геттер направления ветра
float classInformationSensor::getWindDirection() const{
    return windDirection;
}

// сеттер времени
void classInformationSensor::setTime(const QDateTime& time){
    this->time = time;
}

// сеттер имени сенсора
void classInformationSensor::setNameSensor(const QString& name){
    this->nameSensor = name;
}

// сеттер скорости
void classInformationSensor::setWindSpeed(float speed){
    this->windSpeed = speed;
}

// сеттер направления
void classInformationSensor::setWindDirection(float direction){
    this->windDirection = direction;
}

// сеттер всех параметров
void classInformationSensor::setInformation(const QDateTime& time, const QString& name, float speed, float direction){
    setTime(time);
    setNameSensor(name);
    setWindSpeed(speed);
    setWindDirection(direction);
}

// в строку
QString classInformationSensor::classToString() const{
    return QString("Time: %1, Sensor: %2, Wind Speed: %3, Wind Direction: %4")
        .arg(time.toString("hh:mm:ss"))
        .arg(nameSensor)
        .arg(QString::number(windSpeed, 'f', 2))
        .arg(QString::number(windDirection, 'f', 2));
}
