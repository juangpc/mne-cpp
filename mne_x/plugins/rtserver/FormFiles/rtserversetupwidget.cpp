//=============================================================================================================
/**
* @file     rtserversetupwidget.cpp
* @author   Christoph Dinh <chdinh@nmr.mgh.harvard.edu>;
*           Matti Hamalainen <msh@nmr.mgh.harvard.edu>
* @version  1.0
* @date     February, 2013
*
* @section  LICENSE
*
* Copyright (C) 2013, Christoph Dinh and Matti Hamalainen. All rights reserved.
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
* @brief    Contains the implementation of the RTServerSetupWidget class.
*
*/

//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================

#include "rtserversetupwidget.h"
#include "rtserveraboutwidget.h"
#include "../rtserver.h"


//*************************************************************************************************************
//=============================================================================================================
// QT INCLUDES
//=============================================================================================================

#include <QDir>
#include <QDebug>


//*************************************************************************************************************
//=============================================================================================================
// USED NAMESPACES
//=============================================================================================================

using namespace RtServerPlugin;


//*************************************************************************************************************
//=============================================================================================================
// DEFINE MEMBER METHODS
//=============================================================================================================

RtServerSetupWidget::RtServerSetupWidget(RtServer* p_pRtServer, QWidget* parent)
: QWidget(parent)
, m_pRtServer(p_pRtServer)
{
    ui.setupUi(this);

    //rt server connection
    this->ui.m_qLineEdit_Ip->setText(m_pRtServer->m_sRtServerIP);

    connect(ui.m_qPushButton_Connect, &QPushButton::released, this, &RtServerSetupWidget::pressedConnect);

    connect(m_pRtServer, &RtServer::cmdConnectionChanged, this, &RtServerSetupWidget::cmdConnectionChanged);

    //rt server fiffInfo received
    connect(m_pRtServer, &RtServer::fiffInfoAvailable, this, &RtServerSetupWidget::fiffInfoReceived);

    //Fiff Record File
    connect(ui.m_qPushButton_FiffRecordFile, &QPushButton::released, this, &RtServerSetupWidget::pressedFiffRecordFile);

    //Buffer
    connect(ui.m_qLineEdit_BufferSize, &QLineEdit::editingFinished, this, &RtServerSetupWidget::bufferSizeEdited);

    //CLI
    connect(ui.m_qPushButton_SendCLI, &QPushButton::released, this, &RtServerSetupWidget::pressedSendCLI);

    //About
    connect(ui.m_qPushButton_About, &QPushButton::released, this, &RtServerSetupWidget::showAboutDialog);

    this->init();
}


//*************************************************************************************************************

RtServerSetupWidget::~RtServerSetupWidget()
{

}


//*************************************************************************************************************

void RtServerSetupWidget::init()
{
    cmdConnectionChanged(m_pRtServer->m_bCmdClientIsConnected);
}


//*************************************************************************************************************

void RtServerSetupWidget::bufferSizeEdited()
{
    bool t_bSuccess = false;
    qint32 t_iBufferSize = ui.m_qLineEdit_BufferSize->text().toInt(&t_bSuccess);

    if(t_bSuccess && t_iBufferSize > 0)
        m_pRtServer->m_iBufferSize = t_iBufferSize;
    else
        ui.m_qLineEdit_BufferSize->setText(QString("%1").arg(m_pRtServer->m_iBufferSize));
}


//*************************************************************************************************************

void RtServerSetupWidget::pressedFiffRecordFile()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Fiff Record File"), "", tr("Fiff Record File (*.fif)"));
}


//*************************************************************************************************************

void RtServerSetupWidget::pressedConnect()
{
    if(m_pRtServer->m_bCmdClientIsConnected)
        m_pRtServer->disconnectCmdClient();
    else
    {
        m_pRtServer->m_sRtServerIP = this->ui.m_qLineEdit_Ip->text();
        m_pRtServer->connectCmdClient();
    }
}


//*************************************************************************************************************

void RtServerSetupWidget::pressedSendCLI()
{
    if(m_pRtServer->m_bCmdClientIsConnected)
    {
        this->printToLog(this->ui.m_qLineEdit_SendCLI->text());
        QString t_sReply = m_pRtServer->m_pRtCmdClient->sendCLICommand(this->ui.m_qLineEdit_SendCLI->text());
        this->printToLog(t_sReply);
    }
}


//*************************************************************************************************************

void RtServerSetupWidget::printToLog(QString logMsg)
{
    ui.m_qTextBrowser_ServerMessage->insertPlainText(logMsg+"\n");
    //scroll down to the newest entry
    QTextCursor c = ui.m_qTextBrowser_ServerMessage->textCursor();
    c.movePosition(QTextCursor::End);
    ui.m_qTextBrowser_ServerMessage->setTextCursor(c);
}


//*************************************************************************************************************

void RtServerSetupWidget::cmdConnectionChanged(bool p_bConnectionStatus)
{
    if(p_bConnectionStatus)
    {
        //
        // set frequency txt
        //
        if(!m_pRtServer->m_fiffInfo.isEmpty())
            this->ui.m_qLabel_sps->setText(QString("%1").arg(m_pRtServer->m_fiffInfo.sfreq));

        //
        // set buffer size txt
        //
        this->ui.m_qLineEdit_BufferSize->setText(QString("%1").arg(m_pRtServer->m_iBufferSize));

        //
        // set connectors
        //
        QMap<qint32, QString>::ConstIterator it = m_pRtServer->m_qMapConnectors.begin();
        qint32 idx = 0;

        for(; it != m_pRtServer->m_qMapConnectors.end(); ++it)
        {
            this->ui.m_qComboBox_Connector->insertItem(idx, it.value(), it.key());
            ++idx;
        }

        //UI enables/disables
        this->ui.m_qLabel_ConnectionStatus->setText(QString("Connected"));
        this->ui.m_qLineEdit_Ip->setEnabled(false);
        this->ui.m_qPushButton_Connect->setText(QString("Disconnect"));
        this->ui.m_qLineEdit_SendCLI->setEnabled(true);
        this->ui.m_qPushButton_SendCLI->setEnabled(true);
    }
    else
    {
        //clear connectors --> ToDO create a clear function
        m_pRtServer->m_qMapConnectors.clear();
        this->ui.m_qComboBox_Connector->clear();
        m_pRtServer->m_iBufferSize = -1;

        //UI enables/disables
        this->ui.m_qLabel_ConnectionStatus->setText(QString("Not connected"));
        this->ui.m_qLineEdit_Ip->setEnabled(true);
        this->ui.m_qPushButton_Connect->setText(QString("Connect"));
        this->ui.m_qLineEdit_SendCLI->setEnabled(false);
        this->ui.m_qPushButton_SendCLI->setEnabled(false);

        this->ui.m_qLineEdit_BufferSize->setText(QString(""));

    }
}


//*************************************************************************************************************

void RtServerSetupWidget::fiffInfoReceived()
{
    if(!m_pRtServer->m_fiffInfo.isEmpty())
        this->ui.m_qLabel_sps->setText(QString("%1").arg(m_pRtServer->m_fiffInfo.sfreq));

}


//*************************************************************************************************************

void RtServerSetupWidget::showAboutDialog()
{
    RtServerAboutWidget aboutDialog(this);
    aboutDialog.exec();
}
