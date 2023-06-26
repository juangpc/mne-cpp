//=============================================================================================================
/**
 * @file     fieldline_view.cpp
 * @author   Juan Garcia-Prieto <jgarciaprieto@mgh.harvard.edu>
 *           Gabriel Motta <gbmotta@mgh.harvard.edu>
 * @since    0.1.9
 * @date     February, 2023
 *
 * @section  LICENSE
 *
 * Copyright (C) 2023, Gabriel Motta, Juan Garcia-Prieto. All rights reserved.
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
 * @brief     FieldlineView class definition.
 *
 */

//=============================================================================================================
// QT INCLUDES
//=============================================================================================================

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QTableWidget>
#include <QStringList>
#include <QPushButton>

#include <string>
#include <iostream>

//=============================================================================================================
// EIGEN INCLUDES
//=============================================================================================================

//=============================================================================================================
// INCLUDES
//=============================================================================================================

#include "fieldline/fieldline.h"
#include "fieldline/fieldline_view.h"
#include "fieldline/fieldline_view_chassis.h"
#include "fieldline/fieldline_acqsystem/fieldline_acqsystem.h"
#include "formfiles/ui_fieldline_view.h"

//=============================================================================================================
// DEFINE STATIC METHODS
//=============================================================================================================

//=============================================================================================================
// DEFINE MEMBER METHODS
//=============================================================================================================

using FIELDLINEPLUGIN::FieldlineView;
using FIELDLINEPLUGIN::FieldlineSensorStatusType;

//=============================================================================================================

FieldlineView::FieldlineView(Fieldline *parent)
: m_pFieldlinePlugin(parent),
  m_pUi(new Ui::uiFieldlineView)
{
    m_pUi->setupUi(this);
    initTopMenu();
    initChassisView(2);
//    hideChassisView();
    connect(this, &FieldlineView::updateMacIpTable,
            this, &FieldlineView::updateMacIpTableItem);
}

//=============================================================================================================

FieldlineView::~FieldlineView()
{
    delete m_pUi;
}

//=============================================================================================================

void FieldlineView::setChassisConfiguration(int num_chassis, int num_sensors_per_chassis)
{
    initChassisView(num_chassis, num_sensors_per_chassis);
}

//=============================================================================================================

void FieldlineView::setSensorState(int chassis_id, int sensor_id, FieldlineSensorStatusType state)
{
    m_pAcqSystem[chassis_id]->setSensorState(sensor_id, state);
}

//=============================================================================================================

void FieldlineView::initTopMenu()
{
    m_pUi->topMenuFrame->setEnabled(true);
    m_pUi->numChassisSpinBox->setMinimum(0);
    m_pUi->numChassisSpinBox->setMaximum(6);
    m_pUi->numChassisSpinBox->setValue(0);
    m_pMacIpTable = new QTableWidget(this);
    m_pMacIpTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_pMacIpTable->verticalHeader()->setSectionHidden(0, true);

    m_pMacIpTable->setColumnCount(2);
    m_pMacIpTable->setHorizontalHeaderLabels(QStringList({"Mac", "IP"}));
    m_pMacIpTable->horizontalHeader()->setSortIndicatorShown(false);

    QVBoxLayout* macIpTableLayout = qobject_cast<QVBoxLayout*>(m_pUi->ipMacFrame->layout());
    macIpTableLayout->insertWidget(0, m_pMacIpTable);

    connect(m_pUi->numChassisSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
                     this, &FieldlineView::setNumRowsIpMacFrame);
    connect(m_pUi->findIPsBtn, &QPushButton::clicked,
                     this, &FieldlineView::findIps);
    connect(m_pUi->connectBtn, &QPushButton::clicked,
                     this, &FieldlineView::connectToAcqSys);
    connect(m_pUi->disconnectBtn, &QPushButton::clicked,
                     this, &FieldlineView::disconnectFromAcqSys);
}

//=============================================================================================================

void FieldlineView::hideChassisView()
{
    m_pUi->acqSystemTopButtonsFrame->hide();
    m_pUi->chassisRackFrame->hide();
}

//=============================================================================================================

void FieldlineView::showChassisView()
{
    m_pUi->acqSystemTopButtonsFrame->show();
    m_pUi->chassisRackFrame->show();
}

//=============================================================================================================

void FieldlineView::findIps() {
    std::vector<std::string> macList;
    macList.reserve(m_pMacIpTable->rowCount());
    for (int i = 0; i < m_pMacIpTable->rowCount(); i++) {
       macList.emplace_back(m_pMacIpTable->item(i, 0)->text().toStdString());
    }
    auto callback = [this](std::vector<std::string>& ipList) {
        for (size_t i = 0; i < ipList.size(); i++) {
            emit this->updateMacIpTable(i, 1, QString::fromStdString(ipList[i]));
        }
    };
    m_pFieldlinePlugin->findIpAsync(macList, callback);
}

//=============================================================================================================

void FieldlineView::updateMacIpTableItem(int row, int col, const QString& str) {
    m_pMacIpTable->item(row, col)->setText(str);
}

//=============================================================================================================

void FieldlineView::setNumRowsIpMacFrame(int numRows)
{
    m_pMacIpTable->setRowCount(numRows);
    QStringList vertLabels;
    for ( int i = 0; i < numRows; i++ ) {
        vertLabels.push_back(QString::number(i));
    }
    m_pMacIpTable->setVerticalHeaderLabels(vertLabels);

    if (numRows > 0) {
        m_pMacIpTable->setSortingEnabled(false);
        m_pMacIpTable->setItem(numRows-1, 0, new QTableWidgetItem("a0:b1:c2:d3:e4:f5"));
        m_pMacIpTable->setItem(numRows-1, 1, new QTableWidgetItem("0.0.0.0"));
        m_pMacIpTable->item(numRows-1, 0)->setToolTip((QString("Double click to find the IP.")));
        m_pMacIpTable->setSortingEnabled(true);
    }
}

//=============================================================================================================

void FieldlineView::initAcqSystemTopButtons()
{
    QHBoxLayout* acqSystemTopBtnMenuLayout = qobject_cast<QHBoxLayout*>(m_pUi->acqSystemTopButtonsFrame->layout());
    auto addTopButton = [this, acqSystemTopBtnMenuLayout](const QString& name, int order, void(FieldlineView::*ptr)()){
        QPushButton* button = new QPushButton(name, m_pUi->acqSystemTopButtonsFrame);
        connect(button, &QPushButton::clicked, this, ptr);
        acqSystemTopBtnMenuLayout->insertWidget(order, button);
    };

    addTopButton("Start", 0, &FieldlineView::startAllSensors);
    addTopButton("Stop", 1, &FieldlineView::stopAllSensors);
    //number 2 is skipped because that is a spacer in the layout.
    addTopButton("Auto-Tune", 3, &FieldlineView::autoTuneAllSensors);
    addTopButton("Restart Sensors", 4, &FieldlineView::restartAllSensors);
    addTopButton("Coarse Zero", 5, &FieldlineView::coarseZeroAllSensors);
    addTopButton("Fine Zero", 6, &FieldlineView::fineZeroAllSensors);
    addTopButton("Save Sensor State Report", 7, &FieldlineView::saveSensorStateReport);
}

//=============================================================================================================

void FieldlineView::initChassisView(int numChassis)
{
    initChassisView(numChassis, defaultNumSensorsPerChassis);
}

//=============================================================================================================

void FieldlineView::initChassisView(int numChassis, int numSensorsPerChassis)
{
    initAcqSystemTopButtons();
    QVBoxLayout* acqSystemRackLayout = qobject_cast<QVBoxLayout*>(m_pUi->chassisRackFrame->layout());
    for (int i = 0; i < numChassis; i++) {
        FieldlineViewChassis* pChassis = new FieldlineViewChassis(this, i, numSensorsPerChassis);
        connect(pChassis, &FieldlineViewChassis::restartSensor, [this, i](int sensor){emit restartSensor(i, sensor);});
        connect(pChassis, &FieldlineViewChassis::coarseZeroSensor, [this, i](int sensor){emit coarseZeroSensor(i, sensor);});
        connect(pChassis, &FieldlineViewChassis::fineZeroSensor, [this, i](int sensor){emit fineZeroSensor(i, sensor);});

        acqSystemRackLayout->insertWidget(i, pChassis);
        m_pAcqSystem.push_back(pChassis);
    }
}
