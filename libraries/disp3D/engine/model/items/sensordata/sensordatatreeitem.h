//=============================================================================================================
/**
* @file     sensordatatreeitem.h
* @author   Felix Griesau <felix.griesau@tu-ilmenau.de>;
*           Matti Hamalainen <msh@nmr.mgh.harvard.edu>
* @version  1.0
* @date     June, 2017
*
* @section  LICENSE
*
* Copyright (2017) Year, Felix Griesau and Matti Hamalainen. All rights reserved.
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
*/

#ifndef SENSORDATATREEITEM_H
#define SENSORDATATREEITEM_H


//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================

#include "../../../../disp3D_global.h"
#include "../common/abstracttreeitem.h"
#include "../measurement/measurementtreeitem.h"
#include <fiff/fiff_types.h>

//*************************************************************************************************************
//=============================================================================================================
// QT INCLUDES
//=============================================================================================================

#include <QPointer>


//*************************************************************************************************************
//=============================================================================================================
// Eigen INCLUDES
//=============================================================================================================


//*************************************************************************************************************
//=============================================================================================================
// FORWARD DECLARATIONS
//=============================================================================================================
namespace FIFFLIB{
    class FiffEvoked;
}

namespace MNELIB{
    class MNEBemSurface;
}

//*************************************************************************************************************
//=============================================================================================================
// DEFINE NAMESPACE DISP3DLIB
//=============================================================================================================

namespace DISP3DLIB {


//*************************************************************************************************************
//=============================================================================================================
// DISP3DLIB FORWARD DECLARATIONS
//=============================================================================================================


//=============================================================================================================
/**
* This item allows on-the-fly changes to parameters of visualization. It integrates the features provided in
* GeometryInfo and Interpolation.
*
* @brief This item integrates GeometryInfo and Interpolation into Disp3D structure
*/

class DISP3DSHARED_EXPORT SensorDataTreeItem : public AbstractTreeItem
{
    Q_OBJECT

public:
    typedef QSharedPointer<SensorDataTreeItem> SPtr;            /**< Shared pointer type for sensordatatreeitem. */
    typedef QSharedPointer<const SensorDataTreeItem> ConstSPtr; /**< Const shared pointer type for sensordatatreeitem. */

    //=========================================================================================================
    /**
    * Constructs a sensordatatreeitem object, calls initItem
    */
    explicit SensorDataTreeItem(int iType , const QString& text);

    //=========================================================================================================
    /**
    * Destructor
    */
    virtual ~SensorDataTreeItem();

    //=========================================================================================================
    /**
    * Adds a block actual rt data which is streamed by this item's worker thread item. In order for this function to worker, you must call init(...) beforehand.
    *
    * @param[in] tSensorData                The matrix that holds rt measurement data.
    */
    virtual void addData(const MatrixXd& tSensorData) = 0;

    //=========================================================================================================
    /**      
    * Returns true if this item is initialized.
    *
    * @return                               Returns true if this item is initialized.
    */
    virtual inline bool isDataInit() const;

    //=========================================================================================================
    /**
    * This function sets the loop flag.
    *
    * @param[in] bState                      Whether to loop the data or not.
    */
    virtual void setLoopState(bool bState);

    //=========================================================================================================
    /**
    * This function sets the data streaming.
    *
    * @param[in] bState                      Whether to stream the data to the display or not.
    */
    virtual void setStreamingActive(bool bState);

    //=========================================================================================================
    /**
    * This function sets the time interval for streaming.
    *
    * @param[in] iMSec                      The waiting time inbetween samples.
    */
    virtual void setTimeInterval(int iMSec);

    //=========================================================================================================
    /**
    * This function sets the number of averages.
    *
    * @param[in] iNumberAverages            The new number of averages.
    */
    virtual void setNumberAverages(int iNumberAverages);

    //=========================================================================================================
    /**
    * This function sets the colortable type.
    *
    * @param[in] sColortable                The new colortable ("Hot Negative 1" etc.).
    */
    virtual void setColortable(const QString& sColortable);

    //=========================================================================================================
    /**
    * This function set the normalization value.
    *
    * @param[in] vecThresholds              The new threshold values used for normalizing the data.
    */
    virtual void setNormalization(const QVector3D& vecThresholds);
    
    //=========================================================================================================
    /**
     * This function sets the cancel distance used in distance calculations for the interpolation.
     * Distances higher than this are ignored, i.e. the respective coefficients are set to zero.
     * 
     * @param[in] dCancelDist               The new cancel distance value in meters.
     */
    virtual void setCancelDistance(double dCancelDist);
    
    //=========================================================================================================
    /**
     * This function sets the function that is used in the interpolation process.
     * 
     * @param sInterpolationFunction         Function that computes interpolation coefficients using the distance values.
     */
    virtual void setInterpolationFunction(const QString &sInterpolationFunction);

    //=========================================================================================================
    /**
    * Set the sampling frequency.
    *
    * @param[in] dSFreq                 The new sampling frequency.
    */
    virtual void setSFreq(const double dSFreq) = 0;

    //=========================================================================================================
    /**
    * Update bad channels and recalculate interpolation matrix.
    *
    * @param[in] info                 The fiff info including the new bad channels.
    */
    virtual void updateBadChannels(const FIFFLIB::FiffInfo& info) = 0;

protected:
    //=========================================================================================================
    /**
    * This adds all meta tree items and connects them fittingly
    * Don't use this fucntion in the constructor of the abstract class.
    */
    virtual void initItem() override;

    //=========================================================================================================
    /**
    * This function gets called whenever the check/activation state of the rt data worker changed.
    *
    * @param[in] checkState                 The check state of the worker.
    */
    virtual void onCheckStateWorkerChanged(const Qt::CheckState& checkState) = 0;

    //=========================================================================================================
    /**
    * This function gets called whenever this item receives new color values for each estimated source.
    *
    * @param[in] sourceColorSamples         The color values for each estimated source for left and right hemisphere.
    */
    virtual void onNewRtData(const MatrixX3f &sensorData) = 0;

    //=========================================================================================================
    /**
    * This function gets called whenever the used colormap type changed.
    *
    * @param[in] sColormapType              The name of the new colormap type.
    */
    virtual void onColormapTypeChanged(const QVariant& sColormapType) = 0;

    //=========================================================================================================
    /**
    * This function gets called whenever the time interval in between the streamed samples changed.
    *
    * @param[in] iMSec                      The new time in milliseconds waited in between each streamed sample.
    */
    virtual void onTimeIntervalChanged(const QVariant &iMSec) = 0;

    //=========================================================================================================
    /**
    * This function gets called whenever the normaization value changed. The normalization value is used to normalize the estimated source activation.
    *
    * @param[in] vecThresholds              The new threshold values used for normalizing the data.
    */
    virtual void onDataNormalizationValueChanged(const QVariant &vecThresholds) = 0;

    //=========================================================================================================
    /**
    * This function gets called whenever the check/activation state of the looped streaming state changed.
    *
    * @param[in] checkState                 The check state of the looped streaming state.
    */
    virtual void onCheckStateLoopedStateChanged(const Qt::CheckState& checkState) = 0;

    //=========================================================================================================
    /**
    * This function gets called whenever the number of averages of the streamed samples changed.
    *
    * @param[in] iNumAvr                    The new number of averages.
    */
    virtual  void onNumberAveragesChanged(const QVariant& iNumAvr) = 0;

    //=========================================================================================================
    /**
    * This function gets called whenever the cancel distance of the interpolation changed.
    *
    * @param[in] dCancelDist     The new cancel distance.
    */
    virtual void onCancelDistanceChanged(const QVariant& dCancelDist) = 0;

    //=========================================================================================================
    /**
    * This function gets called whenever the function of the interpolation changed.
    *
    * @param[in] sInterpolationFunction     The new function name.
    */
    virtual void onInterpolationFunctionChanged(const QVariant& sInterpolationFunction) = 0;


    bool                             m_bIsDataInit;                     /**< The init flag. */

    QVector<int>                     m_iUsedSensors;                    /**< Stores the indices of channels inside the passed fiff evoked that are used for interpolation. */
    QVector<int>                     m_iSensorsBad;                     /**< Store bad channel indexes.*/

};


//*************************************************************************************************************
//=============================================================================================================
// INLINE DEFINITIONS
//=============================================================================================================
inline bool SensorDataTreeItem::isDataInit() const
{
    return m_bIsDataInit;
}

} // namespace DISP3DLIB

#endif // SENSORDATATREEITEM_H
