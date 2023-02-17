
#include "new_sensor.h"
#include "new_sensor_global.h"
#include <QLabel>

namespace NEWSENSOR_PLUGIN {

NewSensor::~NewSensor() {

}

QSharedPointer<SCSHAREDLIB::AbstractPlugin> NewSensor::clone() const {
    QSharedPointer<AbstractPlugin> pNewSensor(new NewSensor());
    return pNewSensor;
}

void NewSensor::init() {

}

void NewSensor::unload() {

}

bool NewSensor::start() {
  return true;
}

bool NewSensor::stop() {
  return true;
}

SCSHAREDLIB::AbstractPlugin::PluginType NewSensor::getType() const {
  return SCSHAREDLIB::AbstractPlugin::PluginType::_ISensor;
}

QString NewSensor::getName() const {
  return QString("newish sensor");
}

QWidget* NewSensor::setupWidget() {

  return new QLabel("PEPE");
}

QString NewSensor::getBuildInfo() {

  return QString(buildDateTime()) + QString(" - ")  + QString(buildHash());
}

void NewSensor::run() {

}

}
