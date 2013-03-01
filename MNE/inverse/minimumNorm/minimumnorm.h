//=============================================================================================================
/**
* @file     minimumnorm.h
* @author   Christoph Dinh <chdinh@nmr.mgh.harvard.edu>;
*           Matti Hamalainen <msh@nmr.mgh.harvard.edu>
* @version  1.0
* @date     July, 2012
*
* @section  LICENSE
*
* Copyright (C) 2012, Christoph Dinh and Matti Hamalainen. All rights reserved.
*
* Redistribution and use in source and binary forms, with or without modification, are permitted provided that
* the following conditions are met:
*     * Redistributions of source code must retain the above copyright notice, this list of conditions and the
*       following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and
*       the following disclaimer in the documentation and/or other materials provided with the distribution.
*     * Neither the name of the Massachusetts General Hospital nor the names of its contributors may be used
*       to endorse or promote products derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
* PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL MASSACHUSETTS GENERAL HOSPITAL BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
* PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*
*
* @brief    Minimum norm class declaration.
*
*/

#ifndef MINIMUMNORM_H
#define MINIMUMNORM_H

//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================

#include "../inverse_global.h"
#include "../IInverseAlgorithm.h"

#include <mne/mne_inverse_operator.h>


//*************************************************************************************************************
//=============================================================================================================
// DEFINE NAMESPACE INVERSELIB
//=============================================================================================================

namespace INVERSELIB
{

//*************************************************************************************************************
//=============================================================================================================
// FORWARD DECLARATIONS
//=============================================================================================================


//*************************************************************************************************************
//=============================================================================================================
// USED NAMESPACES
//=============================================================================================================

using namespace MNELIB;


//=============================================================================================================
/**
* Minimum norm estimation algorithm ToDo: Paper references.
*
* @brief Minimum norm estimation
*/
class INVERSESHARED_EXPORT MinimumNorm : public IInverseAlgorithm
{
public:

    //=========================================================================================================
    /**
    * Constructs minimum norm inverse algorithm
    *
    * @param[in] p_inverseOperator  The inverse operator
    * @param[in] lambda             The regularization factor
    * @param[in] method             Use mininum norm, dSPM or sLORETA. ("MNE" | "dSPM" | "sLORETA")
    *
    * @return the prepared inverse operator
    */
    explicit MinimumNorm(const MNEInverseOperator &p_inverseOperator, float lambda, const QString method);

    //=========================================================================================================
    /**
    * Constructs minimum norm inverse algorithm
    *
    * @param[in] p_inverseOperator  The inverse operator
    * @param[in] lambda             The regularization factor
    * @param[in] dSPM               Compute the noise-normalization factors for dSPM?
    * @param[in] sLORETA            Compute the noise-normalization factors for sLORETA?
    *
    * @return the prepared inverse operator
    */
    explicit MinimumNorm(const MNEInverseOperator &p_inverseOperator, float lambda, bool dSPM, bool sLORETA);

    virtual ~MinimumNorm(){}

    //=========================================================================================================
    /**
    * Computes a L2-norm inverse solution Actual code using these principles might be different because the
    * inverse operator is often reused across data sets.
    *
    * @param[in] p_evokedDataSet    Evoked data.
    * @param[out] p_SourceEstimate  The calculated source estimation
    *
    * @return true if successful, false otherwise
    */
    virtual bool calculateInverse(const FiffEvokedDataSet &p_evokedDataSet, SourceEstimate &p_SourceEstimate) const;

    //=========================================================================================================
    /**
    * Set minimum norm algorithm method ("MNE" | "dSPM" | "sLORETA")
    *
    * @param[in] method   Use mininum norm, dSPM or sLORETA.
    */
    void setMethod(QString method);

    //=========================================================================================================
    /**
    * Set minimum norm algorithm method ("MNE" | "dSPM" | "sLORETA")
    *
    * @param[in] dSPM      Compute the noise-normalization factors for dSPM?
    * @param[in] sLORETA   Compute the noise-normalization factors for sLORETA?
    */
    void setMethod(bool dSPM, bool sLORETA);

    //=========================================================================================================
    /**
    * Set regularization factor
    *
    * @param[in] lambda   The regularization factor
    */
    void setRegularization(float lambda);

private:
    MNEInverseOperator m_inverseOperator;   /**< The inverse operator */
    float m_fLambda;                        /**< Regularization parameter */
    QString m_sMethod;                      /**< Selected method */
    bool m_bsLORETA;                        /**< Do sLORETA method */
    bool m_bdSPM;                           /**< Do dSPM method */
};

} //NAMESPACE

#endif // MINIMUMNORM_H
