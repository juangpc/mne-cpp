//=============================================================================================================
/**
* @file     main.cpp
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
* @brief    Implements the main() application function.
*
*/

//*************************************************************************************************************
//=============================================================================================================
// MNE INCLUDES
//=============================================================================================================

#include <fiff/fiff_cov.h>
#include <mne/mne.h>
#include <inverse/sourceestimate.h>
#include <inverse/minimumNorm/minimumnorm.h>


//*************************************************************************************************************
//=============================================================================================================
// STL INCLUDES
//=============================================================================================================

#include <math.h>
#include <iostream>

//DEBUG fstream
#include <fstream>


//*************************************************************************************************************
//=============================================================================================================
// QT INCLUDES
//=============================================================================================================

#include <QtCore/QCoreApplication>


//*************************************************************************************************************
//=============================================================================================================
// USED NAMESPACES
//=============================================================================================================

using namespace FIFFLIB;
using namespace MNELIB;
using namespace INVERSELIB;


//*************************************************************************************************************
//=============================================================================================================
// MAIN
//=============================================================================================================

//=============================================================================================================
/**
* The function main marks the entry point of the program.
* By default, main has the storage class extern.
*
* @param [in] argc (argument count) is an integer that indicates how many arguments were entered on the command line when the program was started.
* @param [in] argv (argument vector) is an array of pointers to arrays of character objects. The array objects are null-terminated strings, representing the arguments that were entered on the command line when the program was started.
* @return the value that was set to exit() (which is 0 if exit() is called via quit()).
*/
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QFile t_fileFwdMeeg("./MNE-sample-data/MEG/sample/sample_audvis-meg-eeg-oct-6-fwd.fif");
    QFile t_fileFwdEeg("./MNE-sample-data/MEG/sample/sample_audvis-eeg-oct-6-fwd.fif");
    QFile t_fileCov("./MNE-sample-data/MEG/sample/sample_audvis-cov.fif");
    QFile t_fileEvoked("./MNE-sample-data/MEG/sample/sample_audvis-ave.fif");

    double snr = 3.0;
    double lambda2 = 1.0 / pow(snr, 2);
    QString method("dSPM"); //"MNE" | "dSPM" | "sLORETA"

    // Load data
    fiff_int_t setno = 0;
    FiffEvokedDataSet evokedSet(t_fileEvoked, setno);

    // ToDo
//    std::cout << "evokedSet:\n" << evokedSet.evoked[0]->epochs.block(0,0,10,10);

    MNEForwardSolution t_forwardMeeg(t_fileFwdMeeg, false, true); //OK - inconsistend with mne-python when reading with surf_ori = true


//    QFile file("D:/Users/Christoph/Desktop/sol_data.txt");
//    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
//        return 1;

//    double v = 0;
//    qint32 row = 0;
//    while (!file.atEnd()) {
//        QString line = file.readLine();
//        QStringList t_qListEntries = line.split(" ");

//        for(qint32 j = 0; j < t_qListEntries.size(); ++j)
//        {
//            v = t_qListEntries[j].toDouble();
//            t_forwardMeeg.sol->data(row,j) = v;
//        }
//        ++row;
//    }
//    file.close();

    FiffCov noise_cov(t_fileCov); //OK

    // regularize noise covariance
    noise_cov = noise_cov.regularize(evokedSet.info, 0.05, 0.05, 0.1, true); //OK

    // Restrict forward solution as necessary for MEG
    MNEForwardSolution t_forwardMeg = t_forwardMeeg.pick_types(true, false);
    // Alternatively, you can just load a forward solution that is restricted
    MNEForwardSolution t_forwardEeg(t_fileFwdEeg, false, true);

    // make an M/EEG, MEG-only, and EEG-only inverse operators
    FiffInfo info = evokedSet.info;

    MNEInverseOperator inverse_operator_meeg = MNEInverseOperator::make_inverse_operator(info, t_forwardMeeg, noise_cov, 0.2, 0.8);
    MNEInverseOperator inverse_operator_meg = MNEInverseOperator::make_inverse_operator(info, t_forwardMeg, noise_cov, 0.2, 0.8);
    MNEInverseOperator inverse_operator_eeg = MNEInverseOperator::make_inverse_operator(info, t_forwardEeg, noise_cov, 0.2, 0.8);

    // Compute inverse solution
    MinimumNorm minimumNorm_meeg(inverse_operator_meeg, lambda2, method);
    SourceEstimate sourceEstimate_meeg;
    if(!minimumNorm_meeg.calculateInverse(evokedSet, sourceEstimate_meeg))
        return 1;

    MinimumNorm minimumNorm_meg(inverse_operator_meg, lambda2, method);
    SourceEstimate sourceEstimate_meg;
    if(!minimumNorm_meg.calculateInverse(evokedSet, sourceEstimate_meg))
        return 1;

    MinimumNorm minimumNorm_eeg(inverse_operator_eeg, lambda2, method);
    SourceEstimate sourceEstimate_eeg;
    if(!minimumNorm_eeg.calculateInverse(evokedSet, sourceEstimate_eeg))
        return 1;

    // View activation time-series
    std::cout << "\nsourceEstimate_meeg:\n" << sourceEstimate_meeg.data.block(0,0,10,10) << std::endl;
    printf("tmin = %f s\n", sourceEstimate_meeg.tmin);
    printf("tstep = %f s\n", sourceEstimate_meeg.tstep);

    std::cout << "\nsourceEstimate_meg:\n" << sourceEstimate_meg.data.block(0,0,10,10) << std::endl;
    printf("tmin = %f s\n", sourceEstimate_meg.tmin);
    printf("tstep = %f s\n", sourceEstimate_meg.tstep);

    std::cout << "\nsourceEstimate_eeg:\n" << sourceEstimate_eeg.data.block(0,0,10,10) << std::endl;
    printf("tmin = %f s\n", sourceEstimate_eeg.tmin);
    printf("tstep = %f s\n", sourceEstimate_eeg.tstep);

    return a.exec();
}
