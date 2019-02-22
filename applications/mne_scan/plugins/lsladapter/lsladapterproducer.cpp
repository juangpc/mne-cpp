//=============================================================================================================
/**
* @file     lsladapterproducer.cpp
* @author   Simon Heinke <simon.heinke@tu-ilmenau.de>
*           Matti Hamalainen <msh@nmr.mgh.harvard.edu>
* @version  1.0
* @date     February, 2019
*
* @section  LICENSE
*
* Copyright (C) 2018, Simon Heinke and Matti Hamalainen. All rights reserved.
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
* @brief    Contains the definition of the LSLAdapterProducer class.
*
*/

//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================

#include "lsladapterproducer.h"


//*************************************************************************************************************
//=============================================================================================================
// QT INCLUDES
//=============================================================================================================

#include <QDebug>
#include <QThread>


//*************************************************************************************************************
//=============================================================================================================
// EIGEN INCLUDES
//=============================================================================================================


//*************************************************************************************************************
//=============================================================================================================
// USED NAMESPACES
//=============================================================================================================

using namespace LSLADAPTERPLUGIN;


//*************************************************************************************************************
//=============================================================================================================
// DEFINE MEMBER METHODS
//=============================================================================================================

LSLAdapterProducer::LSLAdapterProducer(QObject *parent)
    : QObject(parent)
    , m_StreamInfo()
    , m_StreamInlet(Q_NULLPTR)
    , m_bIsRunning(false)
    , m_bHasStreamInfo(false)
{

}


//*************************************************************************************************************

LSLAdapterProducer::~LSLAdapterProducer()
{
    delete m_StreamInlet;
}


//*************************************************************************************************************

void LSLAdapterProducer::readStream()
{   
    // check if we have a stream info
    if (m_bHasStreamInfo == false) {
        qDebug() << "[LSLAdapterProducer::readStream] No stream info was supplied !";
        return;
    }

    // start to stream, build a stream inlet
    m_bIsRunning = true;
    m_StreamInlet = new lsl::stream_inlet(m_StreamInfo);

    while (m_bIsRunning) {

        // DUMMY CODE START ===============
        std::vector<float> sample;
        m_StreamInlet->pull_sample(sample);
        for (const auto& f : sample) {
            qDebug() << f;
        }
        // DUMMY CODE END =================

    }

    // cleanup
    m_StreamInlet->close_stream();

    emit finished();
}


//*************************************************************************************************************

void LSLAdapterProducer::setStreamInfo(const lsl::stream_info& stream)
{
    m_StreamInfo = stream;
    m_bHasStreamInfo = true;
    // @TODO parse fiff info from lsl stream info
}


//*************************************************************************************************************

void LSLAdapterProducer::stop()
{
    m_bIsRunning = false;
}


//*************************************************************************************************************

void LSLAdapterProducer::reset()
{
    // reset flags
    m_bIsRunning = false;
    m_bHasStreamInfo = false;
    // reset lsl members
    m_StreamInfo = lsl::stream_info();
    delete m_StreamInlet;
    m_StreamInlet = Q_NULLPTR;
}
