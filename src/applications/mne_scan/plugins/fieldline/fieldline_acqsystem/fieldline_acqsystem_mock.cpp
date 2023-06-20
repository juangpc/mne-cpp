#include "fieldline_acqsystem.h"

using FIELDLINEPLUGIN::FieldlineAcqSystem;

FieldlineAcqSystem::FieldlineAcqSystem(){}

FieldlineAcqSystem::~FieldlineAcqSystem(){}

void FieldlineAcqSystem::callFunctionAsync(const char* moduleName, const char* funcName){}

void FieldlineAcqSystem::callFunction(const std::string& moduleName, const std::string& funcName){}

void FieldlineAcqSystem::startADC(){}
void FieldlineAcqSystem::stopADC(){}
//Fieldline* m_pControllerParent;
void FieldlineAcqSystem::initSampleArrays(){}
void FieldlineAcqSystem::addSampleToSamplesColumn(size_t sensorIdx, double value){}
void FieldlineAcqSystem::restartAllSensors(){}
void FieldlineAcqSystem::coarseZeroAllSensors(){}
void FieldlineAcqSystem::fineZeroAllSensors(){}
void FieldlineAcqSystem::setDataCallback(){}
void FieldlineAcqSystem::setCloseLoop(){}

void FieldlineAcqSystem::postInitPython() const{}
void FieldlineAcqSystem::runPythonFile(const char* file, const char* comment) const{}
void* FieldlineAcqSystem::loadModule(const char* moduleName){return nullptr;}
void* FieldlineAcqSystem::loadCModule(const char* moduleName, void* (*moduleInitFunc)(void)){return nullptr;}
void* FieldlineAcqSystem::loadSensors(){return nullptr;}
