#ifndef NEWSENSOR_PLUGIN_H
#define NEWSENSOR_PLUGIN_H

#include <scShared/Plugins/abstractsensor.h>
#include <utils/generics/circularbuffer.h>

#include <QObject>

namespace NEWSENSOR_PLUGIN {

  
class NewSensor : public SCSHAREDLIB::AbstractSensor {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID "scsharedlib/1.0" FILE "new_sensor.json") //New Qt5 Plugin system replaces Q_EXPORT_PLUGIN2 macro
  // Use the Q_INTERFACES() macro to tell Qt's meta-object system about the interfaces
  Q_INTERFACES(SCSHAREDLIB::AbstractSensor)
 public:
  NewSensor() = default;

  virtual ~NewSensor();

  virtual QSharedPointer<SCSHAREDLIB::AbstractPlugin> clone() const;

  virtual void init();

  virtual void unload();

  virtual bool start();

  virtual bool stop();

  virtual AbstractPlugin::PluginType getType() const;
  virtual QString getName() const;
  virtual QWidget* setupWidget();
  virtual QString getBuildInfo();

 protected:
  virtual void run();
};




}

#endif  // define NEWSENSOR_PLUGIN_H
