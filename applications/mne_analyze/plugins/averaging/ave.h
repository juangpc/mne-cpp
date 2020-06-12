#ifndef AVE_H
#define AVE_H

//=============================================================================================================
// INCLUDES
//=============================================================================================================

#include <fiff/fiff_evoked_set.h>
#include <fiff/fiff_info.h>

//=============================================================================================================
// QT INCLUDES
//=============================================================================================================

#include <QObject>
#include <QThread>
#include <QSharedPointer>

//=============================================================================================================
// EIGEN INCLUDES
//=============================================================================================================

#include <Eigen/Core>

//=============================================================================================================
// FORWARD DECLARATIONS
//=============================================================================================================

//=============================================================================================================
// DEFINE NAMESPACE RTPROCESSINGLIB
//=============================================================================================================

namespace AVERAGINGPLUGIN
{

//=============================================================================================================
/**
 * Real-time averaging worker
 *
 * @brief Real-time averaging worker
 */
class AveWorker : public QObject
{
    Q_OBJECT

public:
    //=========================================================================================================
    /**
     * Creates the real-time averaging object.
     *
     * @param[in] numAverages            Number of evkos to average
     * @param[in] iPreStimSamples      Number of samples averaged before the stimulus
     * @param[in] iPostStimSamples     Number of samples averaged after the stimulus (including the stimulus)
     * @param[in] iBaselineFromSecs    Start of baseline area which was/is used for correction in msecs
     * @param[in] iBaselineToSSecs     End of baseline area which was/is used for correction in msecs
     * @param[in] iTriggerIndex        Row in dex of channel which is to be scanned for triggers
     * @param[in] pFiffInfo            Associated Fiff Information
     */
    AveWorker(quint32 numAverages,
                quint32 iPreStimSamples,
                quint32 iPostStimSamples,
                quint32 iBaselineFromSecs,
                quint32 iBaselineToSecs,
                quint32 iTriggerIndex,
                FIFFLIB::FiffInfo::SPtr pFiffInfo);

    //=========================================================================================================
    /**
     * Perform one single HPI fit.
     *
     * @param[in] t_mat           Data to estimate the HPI positions from
     */
    void doWork(const Eigen::MatrixXd& matData);

    //=========================================================================================================
    /**
     * Sets the number of averages
     *
     * @param[in] numAve     new number of averages
     */
    void setAverageNumber(qint32 numAve);

    //=========================================================================================================
    /**
     * Sets the number of pre stimulus samples
     *
     * @param[in] samples    new number of pre stimulus samples
     * @param[in] secs    new number of pre stimulus seconds
     */
    void setPreStim(qint32 samples,
                    qint32 secs);

    //=========================================================================================================
    /**
     * Sets the number of post stimulus samples
     *
     * @param[in] samples    new number of post stimulus samples
     * @param[in] secs    new number of pre stimulus seconds
     */
    void setPostStim(qint32 samples,
                     qint32 secs);

    //=========================================================================================================
    /**
     * Sets the index of the trigger channel which is to be scanned fo triggers
     *
     * @param[in] idx    trigger channel index
     */
    void setTriggerChIndx(qint32 idx);

    //=========================================================================================================
    /**
     * Sets the artifact reduction
     *
     * @param[in] mapThresholds       The new map including the current thresholds for the channels
     */
    void setArtifactReduction(const QMap<QString, double> &mapThresholds);

    //=========================================================================================================
    /**
     * Sets the baseline correction on or off
     *
     * @param[in] activate    activate baseline correction
     */
    void setBaselineActive(bool activate);

    //=========================================================================================================
    /**
     * Sets the from mSeconds of the baseline area
     *
     * @param[in] fromSamp    from of baseline area in samples
     * @param[in] fromMSec    from of baseline area in mSeconds
     */
    void setBaselineFrom(int fromSamp,
                         int fromMSec);

    //=========================================================================================================
    /**
     * Sets the to mSeconds of the baseline area
     *
     * @param[in] toSamp    to of baseline area in samples
     * @param[in] toMSec    to of baseline area in mSeconds
     */
    void setBaselineTo(int toSamp,
                       int toMSec);

    //=========================================================================================================
    /**
     * Resets the averaged data stored.
     */
    void reset();

    void setTriggerList(QSharedPointer<QList<QPair<int,double>>> lDetectedTriggers, int iOffsetIndex);

protected:
    //=========================================================================================================
    /**
     * do the actual averaging here.
     */
    void doAveraging(const Eigen::MatrixXd& rawSegment);

    //=========================================================================================================
    /**
     * Prepends incoming data to front/pre stim buffer.
     */
    void fillFrontBuffer(const Eigen::MatrixXd& data, double dTriggerType);

    void emitEvoked(double dTriggerType, QStringList& lResponsibleTriggerTypes);

    //=========================================================================================================
    /**
     * Prepends incoming data to back/post stim buffer.
     */
    void fillBackBuffer(const Eigen::MatrixXd& data, double dTriggerType);

    //=========================================================================================================
    /**
     * Packs the buffers togehter as one and calcualtes the current running average and emits the result if number of averages has been reached.
     */
    void mergeData(double dTriggerType);

    //=========================================================================================================
    /**
     * Generates the final evoke variable.
     */
    void generateEvoked(double dTriggerType);

    //=========================================================================================================
    /**
     * Check if control values have been changed
     */
    inline bool controlValuesChanged();

    QSharedPointer<QList<QPair<int,double>>>        m_pDetectedTriggers;

    qint32                                          m_iNumAverages;             /**< Number of averages */

    qint32                                          m_iPreStimSamples;          /**< Amount of samples averaged before the stimulus. */
    qint32                                          m_iNewPreStimSamples;       /**< New amount of samples averaged before the stimulus. */

    qint32                                          m_iPostStimSamples;         /**< Amount of samples averaged after the stimulus, including the stimulus sample.*/
    qint32                                          m_iNewPostStimSamples;      /**< New amount of samples averaged after the stimulus, including the stimulus sample.*/

    qint32                                          m_iTriggerChIndex;          /**< Current row index of the data matrix which is to be scanned for triggers */
    qint32                                          m_iNewTriggerIndex;         /**< Old row index of the data matrix which is to be scanned for triggers */

    float                                           m_fTriggerThreshold;        /**< Threshold to detect trigger */

    bool                                            m_bActivateThreshold;       /**< Whether to do threshold artifact reduction or not. */

    bool                                            m_bDoBaselineCorrection;    /**< Whether to perform baseline correction. */

    QPair<QVariant,QVariant>                        m_pairBaselineSec;          /**< Baseline information in seconds form where the seconds are seen relative to the trigger, meaning they can also be negative [from to]*/
    QPair<QVariant,QVariant>                        m_pairBaselineSamp;         /**< Baseline information in samples form where the seconds are seen relative to the trigger, meaning they can also be negative [from to]*/

    FIFFLIB::FiffInfo::SPtr                         m_pFiffInfo;                /**< Holds the fiff measurement information. */
    FIFFLIB::FiffEvokedSet                          m_stimEvokedSet;            /**< Holds the evoked information. */

    QMap<QString,double>                            m_mapThresholds;            /**< Holds the current thresholds for artifact rejection. */
    QMap<double,QList<Eigen::MatrixXd> >            m_mapStimAve;               /**< the current stimulus average buffer. Holds m_iNumAverages vectors */
    QMap<double,Eigen::MatrixXd>                    m_mapDataPre;               /**< The matrix holding pre stim data. */
    QMap<double,Eigen::MatrixXd>                    m_mapDataPost;              /**< The matrix holding post stim data. */
    QMap<double,qint32>                             m_mapMatDataPostIdx;        /**< Current index inside of the matrix m_matDataPost */
    QMap<double,bool>                               m_mapFillingBackBuffer;     /**< Whether the back buffer is currently getting filled. */

signals:
    //=========================================================================================================
    /**
     * Signal which is emitted when new evoked stimulus data are available.
     *
     * @param[in] evokedStimSet              The evoked stimulus data set.
     * @param[in] lResponsibleTriggerTypes   List of all trigger types which lead to the recent emit of a new evoked set.
     */
    void resultReady(const FIFFLIB::FiffEvokedSet& evokedStimSet,
                     const QStringList& lResponsibleTriggerTypes);
};

//=============================================================================================================
/**
 * Real-time averaging
 *
 * @brief Real-time averaging
 */
class Ave : public QObject
{
    Q_OBJECT

public:
    typedef QSharedPointer<Ave> SPtr;             /**< Shared pointer type for Ave. */
    typedef QSharedPointer<const Ave> ConstSPtr;  /**< Const shared pointer type for Ave. */

    //=========================================================================================================
    /**
     * Creates the real-time averaging object.
     *
     * @param[in] numAverages            Number of evkos to average
     * @param[in] iPreStimSamples      Number of samples averaged before the stimulus
     * @param[in] iPostStimSamples     Number of samples averaged after the stimulus (including the stimulus)
     * @param[in] iBaselineFromSecs    Start of baseline area which was/is used for correction in msecs
     * @param[in] iBaselineToSSecs     End of baseline area which was/is used for correction in msecs
     * @param[in] iTriggerIndex        Row in dex of channel which is to be scanned for triggers
     * @param[in] pFiffInfo            Associated Fiff Information
     * @param[in] parent     Parent QObject (optional)
     */
    explicit Ave(quint32 numAverages,
                   quint32 iPreStimSamples,
                   quint32 iPostStimSamples,
                   quint32 iBaselineFromSecs,
                   quint32 iBaselineToSecs,
                   quint32 iTriggerIndex,
                   FIFFLIB::FiffInfo::SPtr pFiffInfo,
                   QSharedPointer<QList<QPair<int,double>>> lDetectedTriggers,
                   QObject *parent = 0);

    //=========================================================================================================
    /**
     * Destroys the real-time averaging object.
     */
    ~Ave();

    //=========================================================================================================
    /**
     * Slot to receive incoming data.
     *
     * @param[in] data  Data to calculate the average from
     */
    void append(const Eigen::MatrixXd &data);

    //=========================================================================================================
    /**
     * Restarts the thread by interrupting its computation queue, quitting, waiting and then starting it again.
     *
     * @param[in] numAverages            Number of evkos to average
     * @param[in] iPreStimSamples      Number of samples averaged before the stimulus
     * @param[in] iPostStimSamples     Number of samples averaged after the stimulus (including the stimulus)
     * @param[in] iBaselineFromSecs    Start of baseline area which was/is used for correction in msecs
     * @param[in] iBaselineToSSecs     End of baseline area which was/is used for correction in msecs
     * @param[in] iTriggerIndex        Row in dex of channel which is to be scanned for triggers
     * @param[in] pFiffInfo            Associated Fiff Information
     */
    void restart(quint32 numAverages,
                 quint32 iPreStimSamples,
                 quint32 iPostStimSamples,
                 quint32 iBaselineFromSecs,
                 quint32 iBaselineToSecs,
                 quint32 iTriggerIndex,
                 QSharedPointer<QList<QPair<int,double>>> lDetectedTriggers,
                 FIFFLIB::FiffInfo::SPtr pFiffInfo);

    //=========================================================================================================
    /**
     * Stops the thread by interrupting its computation queue, quitting and waiting.
     */
    void stop();

    //=========================================================================================================
    /**
     * Sets the number of averages
     *
     * @param[in] numAve     new number of averages
     */
    void setAverageNumber(qint32 numAve);

    //=========================================================================================================
    /**
     * Sets the number of pre stimulus samples
     *
     * @param[in] samples    new number of pre stimulus samples
     * @param[in] secs    new number of pre stimulus seconds
     */
    void setPreStim(qint32 samples,
                    qint32 secs);

    //=========================================================================================================
    /**
     * Sets the number of post stimulus samples
     *
     * @param[in] samples    new number of post stimulus samples
     * @param[in] secs    new number of pre stimulus seconds
     */
    void setPostStim(qint32 samples,
                     qint32 secs);

    //=========================================================================================================
    /**
     * Sets the index of the trigger channel which is to be scanned fo triggers
     *
     * @param[in] idx    trigger channel index
     */
    void setTriggerChIndx(qint32 idx);

    //=========================================================================================================
    /**
     * Sets the artifact reduction
     *
     * @param[in] mapThresholds       The new map including the current thresholds for the channels
     */
    void setArtifactReduction(const QMap<QString, double> &mapThresholds);

    //=========================================================================================================
    /**
     * Sets the baseline correction on or off
     *
     * @param[in] activate    activate baseline correction
     */
    void setBaselineActive(bool activate);

    //=========================================================================================================
    /**
     * Sets the from mSeconds of the baseline area
     *
     * @param[in] fromSamp    from of baseline area in samples
     * @param[in] fromMSec    from of baseline area in mSeconds
     */
    void setBaselineFrom(int fromSamp,
                         int fromMSec);

    //=========================================================================================================
    /**
     * Sets the to mSeconds of the baseline area
     *
     * @param[in] toSamp    to of baseline area in samples
     * @param[in] toMSec    to of baseline area in mSeconds
     */
    void setBaselineTo(int toSamp,
                       int toMSec);

    //=========================================================================================================
    /**
     * Reset the data processing in the real-time worker
     */
    void reset();


    //=========================================================================================================
    void setTriggerList(QList<QPair<int,double>>* lDetectedTriggers, int iOffsetIndex = 0);

protected:
    //=========================================================================================================
    /**
     * Handles the results.
     */
    void handleResults(const FIFFLIB::FiffEvokedSet& evokedStimSet,
                       const QStringList& lResponsibleTriggerTypes);

    QThread             m_workerThread;         /**< The worker thread. */

signals:
    void evokedStim(const FIFFLIB::FiffEvokedSet& evokedStimSet,
                    const QStringList& lResponsibleTriggerTypes);
    void operate(const Eigen::MatrixXd& matData);
    void averageNumberChanged(qint32 numAve);
    void averagePreStimChanged(qint32 samples,
                               qint32 secs);
    void averagePostStimChanged(qint32 samples,
                                qint32 secs);
    void averageTriggerChIdxChanged(qint32 idx);
    void averageArtifactReductionChanged(const QMap<QString, double> &mapThresholds);
    void averageBaselineActiveChanged(bool activate);
    void averageBaselineFromChanged(int fromSamp,
                                    int fromMSec);
    void averageBaselineToChanged(int toSamp,
                                  int toMSec);
    void averageResetRequested();
};

//=============================================================================================================
// INLINE DEFINITIONS
//=============================================================================================================

inline bool AveWorker::controlValuesChanged()
{
    bool result = false;

    if(m_iNewPreStimSamples != m_iPreStimSamples
       || m_iNewPostStimSamples != m_iPostStimSamples
       || m_iNewTriggerIndex != m_iTriggerChIndex) {
        result = true;
    }

    return result;
}
} //namespace end
#endif // AVE_H