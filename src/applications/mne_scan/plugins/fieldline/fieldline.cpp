//=============================================================================================================
/**
 * @file     fieldline.cpp
 * @author   Juan GarciaPrieto <jgarciaprieto@mgh.harvard.edu>;
 *           Gabriel B Motta <gbmotta@mgh.harvard.edu>;
 * @since    0.1.0
 * @date     February, 2023
 *
 * @section  LICENSE
 *
 * Copyright (C) 2023, Juan G Prieto, Gabriel B Motta. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *     * Neither the name of MNE-CPP authors nor the names of its contributors
 * may be used to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 *
 * @brief    Contains the definition of the Fieldline class.
 *
 */

//=============================================================================================================
// INCLUDES
//=============================================================================================================
#include <string>
#include <vector>
#include <thread>

#include "fieldline/fieldline.h"
#include "fieldline/fieldline_acqsystem/fieldline_acqsystem.h"
#include "fieldline/fieldline_view.h"
#include "fieldline/ipfinder.h"

#include <scMeas/realtimemultisamplearray.h>
#include <utils/generics/circularbuffer.h>

//=============================================================================================================
// QT INCLUDES
//=============================================================================================================

#include <QLabel>
#include <QFileDialog>
#include <QMessageBox>

//=============================================================================================================
// EIGEN INCLUDES
//=============================================================================================================

//=============================================================================================================
// USED NAMESPACES
//=============================================================================================================

namespace FIELDLINEPLUGIN {

//=============================================================================================================

// QSharedPointer<FIFFLIB::FiffInfo> createFiffInfo(int numChassis,
//                                                  int numChannels) {
//   QSharedPointer<FIFFLIB::FiffInfo> pFiffInfo;
//   pFiffInfo->sfreq = 1000.0f;
//   pFiffInfo->nchan = numChassis * numChannels;
//   pFiffInfo->chs.clear();
//
//   for (int chan_i = 0; chan_i < pFiffInfo->nchan; ++chan_i) {
//     FIFFLIB::FiffChInfo channel;
//     channel.ch_name = QString("%1:%2").arg(numChannels, 2).arg(chan_i, 2);
//     channel.kind = FIFFV_MEG_CH;
//     channel.unit = FIFF_UNIT_T;
//     channel.unit_mul = FIFF_UNITM_NONE;
//     channel.chpos.coil_type = FIFFV_COIL_NONE;
//     pFiffInfo->chs.append(channel);
//     pFiffInfo->ch_names.append(channel.ch_name);
//   }
// }

// QSharedPointer<FIFFLIB::FiffInfo>
// createFiffInfo(std::vector<std::vector<int>> fl_chassis, float sfreq = 1000.f) {
//   QSharedPointer<FIFFLIB::FiffInfo> pFiffInfo;
//   pFiffInfo->sfreq = sfreq;
//   pFiffInfo->chs.clear();
//
//   int total_channels = 0;
//   int chassis_num = 0;
//   for (auto &chassis : fl_chassis) {
//     for (auto &sensor : chassis) {
//       FIFFLIB::FiffChInfo channel;
//       channel.ch_name = QString("%1:%2").arg(chassis_num, 2).arg(sensor, 2);
//       channel.kind = FIFFV_MEG_CH;
//       channel.unit = FIFF_UNIT_T;
//       channel.unit_mul = FIFF_UNITM_NONE;
//       channel.chpos.coil_type = FIFFV_COIL_NONE;
//       pFiffInfo->chs.append(channel);
//       pFiffInfo->ch_names.append(channel.ch_name);
//       ++total_channels;
//     }
//   }
//   pFiffInfo->nchan = total_channels;
//
//   return pFiffInfo;
// }
//
// QSharedPointer<FIFFLIB::FiffInfo>
// createFiffInfo(int numChassis, int numChannels, float sfreq = 1000.f) {
//   std::vector<std::vector<int>> fl;
//   for (int i = 0; i < numChassis; ++i) {
//     std::vector<int> ch(numChannels);
//     std::iota(ch.begin(), ch.end(), 1);
//     fl.push_back(std::move(ch));
//   }
//   return createFiffInfo(fl, sfreq);
// }

//=============================================================================================================
// DEFINE MEMBER METHODS
//=============================================================================================================

Fieldline::Fieldline()
: m_pAcqSystem(nullptr)
, m_connected(false)
{
    qDebug() << "Hello Fieldline";
}

//=============================================================================================================

QSharedPointer<SCSHAREDLIB::AbstractPlugin> Fieldline::clone() const
{
    QSharedPointer<SCSHAREDLIB::AbstractPlugin> pFieldlineClone(new Fieldline());
    return pFieldlineClone;
}

//=============================================================================================================

void Fieldline::init()
{
    m_pCircularBuffer = QSharedPointer<UTILSLIB::CircularBuffer_Matrix_double>::create(40);
    m_pRTMSA = SCSHAREDLIB::PluginOutputData
            <SCMEASLIB::RealTimeMultiSampleArray>::create(this, "Fieldline Plugin",
                                                          "FieldlinePlguin output");
    m_pRTMSA->measurementData()->setName(this->getName());
    m_outputConnectors.append(m_pRTMSA);

    m_pAcqSystem = new FieldlineAcqSystem();
}

//=============================================================================================================

void Fieldline::unload() {
    delete m_pAcqSystem;
}

//=============================================================================================================

bool Fieldline::start()
{
    initFiffInfo();

    std::thread t([this]{
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        m_pAcqSystem->startADC();
    });
    t.detach();

    QThread::start();
    return true;
}

//=============================================================================================================

FieldlineView* Fieldline::createView()
{
    auto* view = new FieldlineView(this);

    connect(view, &FieldlineView::connectToAcqSys, this, &Fieldline::connectToAcq);
    connect(view, &FieldlineView::disconnectFromAcqSys, this, &Fieldline::disconnectFromAcq);

    connect(view, &FieldlineView::restartSensor, this, &Fieldline::restartSensor);
    connect(view, &FieldlineView::coarseZeroSensor, this, &Fieldline::coarseZeroSensor);
    connect(view, &FieldlineView::fineZeroSensor, this, &Fieldline::fineZeroSensor);

    connect(view, &FieldlineView::restartAllSensors, this, &Fieldline::restartAllSensors);
    connect(view, &FieldlineView::coarseZeroAllSensors, this, &Fieldline::coarseZeroAllSensors);
    connect(view, &FieldlineView::fineZeroAllSensors, this, &Fieldline::fineZeroAllSensors);

    connect(view, &FieldlineView::saveSensorStateReport, this, &Fieldline::saveSensorStateReport);

    connect(this, &Fieldline::connectedToChassis, view, QOverload<int,int>::of(&FieldlineView::initChassisView));
    connect(this, &Fieldline::disconnectedFromChassis, view, &FieldlineView::clearChassisView);

    return view;
}

//=============================================================================================================

void Fieldline::connectToAcq(QStringList ips)
{
    if(!m_connected && ips.size() > 0){
        qDebug() << "Let's pretend we'e connecting to available chassis.";
        //Add connection logic here

        //This needs to be obtained properly from the acq system.
        const int num_sensors = 16;
        auto default_sensor_state = FieldlineSensorStatusType::SENSOR_OFF;

        m_sensorStatus.clear();
        m_sensorStatus.reserve(ips.size());
        for(auto& ip : ips){
            (void)ip;
            std::vector<FieldlineSensorStatusType> vec;
            vec.reserve(num_sensors);
            for(int i = 0; i < num_sensors; ++i){
                vec.push_back(default_sensor_state);
            }
            m_sensorStatus.push_back(std::move(vec));
        }

        emit connectedToChassis(ips.size(), 16);
        m_connected = true;
    }
}

//=============================================================================================================

void Fieldline::disconnectFromAcq()
{
    if(m_connected){
        emit disconnectedFromChassis();
        m_connected = false;
    }
}

//=============================================================================================================

void Fieldline::restartAllSensors()
{
    //Insert logic to send command to python here.
}

//=============================================================================================================

void Fieldline::coarseZeroAllSensors()
{
    //Insert logic to send command to python here.
}

//=============================================================================================================

void Fieldline::fineZeroAllSensors()
{
    //Insert logic to send command to python here.
}

//=============================================================================================================

void Fieldline::restartSensor(int chassis, int sensor)
{
    //Insert logic to send command to python here.
}

//=============================================================================================================

void Fieldline::coarseZeroSensor(int chassis, int sensor)
{
    //Insert logic to send command to python here.
}

//=============================================================================================================

void Fieldline::fineZeroSensor(int chassis, int sensor)
{
    //Insert logic to send command to python here.
}


//=============================================================================================================

void Fieldline::saveSensorStateReport()
{
    static QString save_dir = "";

    auto saveFilePath = QFileDialog::getSaveFileName(nullptr,
                                                     tr("Save Events"), save_dir,
                                                     tr("Text File (*.txt)"));

    if(saveFilePath.isEmpty()){
        return;
    }

    QFile file(saveFilePath);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "[EventModel::saveToFile] Unable to access file.";
        QMessageBox::warning(0, tr("MNE Scan - Fieldine OPM"), QString("Not able to access file ") + saveFilePath, QMessageBox::Ok);
    }
    QTextStream out(&file);

    // Get values from acq system instead
    QString fake_sensor_fields{"[00:01] x: -19.2 y: 12.6 z: 21.1"};
    QString fake_sensor_cal{"[00:01] 1"};

    out << "MNE Scan - Fieldline OPM - " << QDateTime::currentDateTime().toString() << "\n\n";

    out << "Fields:\n";
    out << fake_sensor_fields << "\n\n";

    out << "Calibration:\n";
    out << fake_sensor_cal;
    out << "\n";

    QFileInfo info(file);
    save_dir = info.absoluteDir().absolutePath();
}

//=============================================================================================================

void Fieldline::initFiffInfo()
{   
    m_pFiffInfo->sfreq = 1000.0f;
    m_pFiffInfo->nchan = 3;
    m_pFiffInfo->chs.clear();
    for (int chan_i = 0; chan_i < m_pFiffInfo->nchan; chan_i++) {
        FIFFLIB::FiffChInfo channel;
        channel.kind = FIFFV_MEG_CH;
        channel.unit = FIFF_UNIT_T;
        channel.unit_mul = FIFF_UNITM_NONE;
        channel.chpos.coil_type = FIFFV_COIL_NONE;
        std::string channel_name(std::string("Ch. ") + std::to_string(chan_i));
        channel.ch_name = QString::fromStdString(channel_name);
        m_pFiffInfo->chs.append(channel);
        m_pFiffInfo->ch_names.append(QString::fromStdString(channel_name));
    }

    m_pRTMSA->measurementData()->initFromFiffInfo(m_pFiffInfo);
    m_pRTMSA->measurementData()->setMultiArraySize(1);
    m_pRTMSA->measurementData()->setVisibility(true);
}

//=============================================================================================================

bool Fieldline::stop()
{
    m_pAcqSystem->stopADC();

    requestInterruption();
    wait(500);

    m_pRTMSA->measurementData()->clear();
    m_pCircularBuffer->clear();

    return true;
}

//=============================================================================================================

SCSHAREDLIB::AbstractPlugin::PluginType Fieldline::getType() const
{
    return SCSHAREDLIB::AbstractPlugin::PluginType::_ISensor;
}

//=============================================================================================================

QString Fieldline::getName() const {
    return QString("Fieldline OPM");
}

//=============================================================================================================

QWidget *Fieldline::setupWidget()
{
    return createView();
}

//=============================================================================================================

void Fieldline::findIpAsync(std::vector<std::string>& macList,
                            std::function<void(std::vector<std::string>&)> callback)
{
    std::thread ipFinder([macList, callback] {
        IPFINDER::IpFinder ipFinder;
        for (auto& mac : macList) {
            ipFinder.addMacAddress(mac);
        }
        ipFinder.findIps();
        std::vector<std::string> ipList;
        ipList.reserve(ipFinder.macIpList.size());
        for (size_t i = 0; i < ipFinder.macIpList.size(); i++) {
            ipList.emplace_back(ipFinder.macIpList[i].ip);
        }
        callback(ipList);
    });
    ipFinder.detach();
}

//=============================================================================================================

void Fieldline::run()
{
    // Eigen::MatrixXd matData;
    // matData.resize(m_pFiffInfo->nchan, 200);
    //
    // while (!isInterruptionRequested()) {
    //   if (m_pCircularBuffer->pop(matData)) {
    //     if (!isInterruptionRequested()) {
    //       // matData = Eigen::MatrixXd::Random(m_pFiffInfo->nchan, 200);
    //       matData *= 4e-12;
    //       // msleep(200);
    //       m_pRTMSA->measurementData()->setValue(matData);
    //     }
    //   }
    // }
}

//=============================================================================================================

QString Fieldline::getBuildInfo()
{
    return QString(buildDateTime()) + QString(" - ") + QString(buildHash());
}

//=============================================================================================================

void Fieldline::newData(double* data, size_t numChannels, size_t numSamples)
{
    Eigen::MatrixXd matData;
    matData.resize(numChannels, numSamples);
    for (size_t i = 0; i < numChannels; i++) {
        for (size_t j = 0; j < numSamples; j++) {
            matData(i, j) = data[i * numSamples + j];
        }
    }
    m_pRTMSA->measurementData()->setValue(matData);
    //
    // while (!m_pCircularBuffer->push(matData)) {
    // }
}

}  // namespace FIELDLINEPLUGIN
