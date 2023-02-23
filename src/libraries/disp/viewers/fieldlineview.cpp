//=============================================================================================================
/**
 * @file     fl_rack.cpp
 * @author   Gabriel Motta <gbmotta@mgh.harvard.edu>
 *           Juan Garcia-Prieto <juangpc@gmail.com>
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
// INCLUDES
//=============================================================================================================

#include "fieldlineview.h"

#include "ui_fl_rack.h"
#include "ui_fl_chassis.h"


//=============================================================================================================
// QT INCLUDES
//=============================================================================================================

//=============================================================================================================
// EIGEN INCLUDES
//=============================================================================================================

//=============================================================================================================
// USED NAMESPACES
//=============================================================================================================

using DISPLIB::FieldlineView;
using DISPLIB::fl_chassis;

//=============================================================================================================
// DEFINE STATIC METHODS
//=============================================================================================================

int FieldlineView::default_num_sensors = 16;

//=============================================================================================================
// DEFINE MEMBER METHODS
//=============================================================================================================


FieldlineView::FieldlineView(int num_chassis, int sensors_per_chassis, QWidget *parent)
: FieldlineView(parent)
{
    configure(num_chassis, sensors_per_chassis);
}

//=============================================================================================================

FieldlineView::FieldlineView(int num_chassis, QWidget *parent)
: FieldlineView(num_chassis, default_num_sensors, parent)
{
}

//=============================================================================================================

FieldlineView::FieldlineView(QWidget *parent)
: QWidget(parent)
, ui(std::make_unique<Ui::fl_rack>())
{
    ui->setupUi(this);
}

//=============================================================================================================

FieldlineView::~FieldlineView()
{
}

//=============================================================================================================

void FieldlineView::configure(int num_chassis)
{
    configure(num_chassis, default_num_sensors);
}

//=============================================================================================================

void FieldlineView::configure(int num_chassis, int num_sensors)
{

    ui->frame->layout()->addWidget(new fl_chassis());
}

//=============================================================================================================

void FieldlineView::setColor(int chassis_id, int sensnor_num, QColor color)
{

}

//=============================================================================================================

void FieldlineView::setColor(int chassis_id, int sensnor_num, QColor color, bool blinking)
{

}

//=============================================================================================================

void FieldlineView::setChassisColor(int chassis_id, QColor color)
{

}

//=============================================================================================================

void FieldlineView::setChassisColor(int chassis_id, QColor color, bool blinking)
{

}

//=============================================================================================================

void FieldlineView::setAllColor(QColor color)
{

}

//=============================================================================================================

void FieldlineView::setAllColor(QColor color, bool blinking)
{

}

//=============================================================================================================

void FieldlineView::setBlinkState(int chassis_id, int sensnor_num, bool blinking)
{

}

//=============================================================================================================

void FieldlineView::setChassisBlinkState(int chassis_id, bool blinking)
{

}

//=============================================================================================================

void FieldlineView::setAllBlinkState(bool blinking)
{

}

//=============================================================================================================

void FieldlineView::setDefaultNumSensors(int num_sensors)
{

}

//=============================================================================================================

fl_chassis::fl_chassis(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::fl_chassis)
{
    ui->setupUi(this);

    for(int i = 0; i < 16; ++i){
        sensors.push_back(std::make_unique<fl_sensor>());
        sensors.back()->setLabel(QString::number(i+1));
        ui->sensor_frame->layout()->addWidget(sensors.back().get());
    }
    sensors.back()->setBlink(true);
}

//=============================================================================================================

fl_chassis::~fl_chassis()
{
    delete ui;
}