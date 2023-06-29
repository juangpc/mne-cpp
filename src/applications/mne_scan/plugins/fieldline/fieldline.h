//=============================================================================================================
/**
 * @file     fieldline.h
 * @author   Juan GarciaPrieto <jgarciaprieto@mgh.harvard.edu>;
 *           Gabriel B Motta <gbmotta@mgh.harvard.edu>;
 * @since    0.1.0
 * @date     February, 2023
 *
 * @section  LICENSE
 *
 * Copyright (C) 2023, Juan G Prieto, Gabriel B Motta. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that
 * the following conditions are met:
 *     * Redistributions of source code must retain the above copyright notice, this list of conditions and the
 *       following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and
 *       the following disclaimer in the documentation and/or other materials provided with the distribution.
 *     * Neither the name of MNE-CPP authors nor the names of its contributors may be used
 *       to endorse or promote products derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 *
 * @brief    Contains the declaration of the Fieldline plugin class.
 *
 */

#ifndef FIELDLINEPLUGIN_FIELDLINE_H
#define FIELDLINEPLUGIN_FIELDLINE_H

//=============================================================================================================
// INCLUDES
//=============================================================================================================

#include "fieldline/fieldline_global.h"

#include "fieldline_definitions.h"

#include <fiff/fiff.h>
#include <fiff/fiff_info.h>
#include <utils/generics/circularbuffer.h>

#include <scShared/Plugins/abstractsensor.h>

#include <QObject>
#include <QSharedPointer>

#include <string>
#include <vector>

//=============================================================================================================
// FORWARD DECLARATION
//=============================================================================================================

namespace SCMEASLIB {
class RealTimeMultiSampleArray;
}

namespace FIFFLIB {
class FiffInfo;
}

//=============================================================================================================
// DEFINE NAMESPACE FIELDLINEPLUGIN
//=============================================================================================================

namespace FIELDLINEPLUGIN {

class FieldlineAcqSystem;
class FieldlineView;
class IpFinder;

//============================================================================================================
/**
 * The Fieldline class provides a MEG connector for receiving data from Fieldline box through its Python API.
 *
 * @brief The Fieldline class provides a MEG connector for receiving data from Fieldline API.
 */
class FIELDLINESHARED_EXPORT Fieldline : public SCSHAREDLIB::AbstractSensor
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "scsharedlib/1.0" FILE "fieldline.json")
    Q_INTERFACES(SCSHAREDLIB::AbstractSensor)

public:
    Fieldline();

    //=========================================================================================================
    // AbstractSensor virtual methods
    virtual QSharedPointer<SCSHAREDLIB::AbstractPlugin> clone() const override;
    virtual void init() override;
    virtual void unload() override;
    virtual bool start() override;
    virtual bool stop() override;
    virtual PluginType getType() const override;
    virtual QString getName() const override;
    virtual QWidget* setupWidget() override;
    virtual QString getBuildInfo() override;

    void findIpAsync(std::vector<std::string>& macList,
                     std::function<void(std::vector<std::string>&)> callback);

    void newData(double* data, size_t numChannels, size_t numSamples);

    void updateSensorState(int chassis, int sensor, FieldlineSensorStatusType state);

signals:
    void connectedToChassis(int numChassis, int numChannels);
    void disconnectedFromChassis();

protected:
    virtual void run() override;

private:
    FieldlineView* createView();

    void connectToAcq(QStringList ips);
    void disconnectFromAcq();

    void restartAllSensors();
    void coarseZeroAllSensors();
    void fineZeroAllSensors();

    void restartSensor(int chassis, int sensor);
    void coarseZeroSensor(int chassis, int sensor);
    void fineZeroSensor(int chassis, int sensor);

    void saveSensorStateReport();

    void initFiffInfo();

    FieldlineAcqSystem* m_pAcqSystem;

    bool m_connected;

    std::vector<std::vector<FieldlineSensorStatusType> > m_sensorStatus;

    QSharedPointer<SCSHAREDLIB::PluginOutputData<SCMEASLIB::RealTimeMultiSampleArray> >  m_pRTMSA;     /**< The RealTimeSampleArray to provide the EEG data.*/
    QSharedPointer<FIFFLIB::FiffInfo> m_pFiffInfo;  /**< Fiff measurement info.*/
    QSharedPointer<UTILSLIB::CircularBuffer_Matrix_double> m_pCircularBuffer;  /**< Holds incoming raw data. */
};

}  // namespace FIELDLINEPLUGIN

#endif  // FIELDLINEPLUGIN_FIELDLINE_H

