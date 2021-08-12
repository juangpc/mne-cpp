
//=============================================================================================================
/**
 * @file     test_commandlineoptionsparser.cpp
 * @author   Juan GPC <jgarciaprieto@mgh.harvard.edu>;
 * @since    0.1.0
 * @date     August, 2021
 *
 * @section  LICENSE
 *
 * Copyright (C) 2021, Juan GPC. All rights reserved.
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
 * @brief    Test for the command line options parser classes
 *
 */

//=============================================================================================================
// INCLUDES
//=============================================================================================================

#include <utils/generics/applicationlogger.h>

#include "core/core_global.h"
#include "core/commandlineoption.h"
#include "core/commandlineoptionsparser.h"

#include <QtCore/QCoreApplication>
#include <QTest>

//=============================================================================================================
/**
 * DECLARE CLASS TestCommandLineOptionsParser
 *
 * Provides CommandLineParser verification tests.
 *
 */
class TestCommandLineOptionsParser: public QObject
{
    Q_OBJECT

public:
    TestCommandLineOptionsParser() = default;

private slots:
    void initTestCase();

    //test CommandLineOption
    void testCommandLineOptionType();
//    void testCommandLineOption

    void cleanupTestCase();

};

//=============================================================================================================

void TestCommandLineOptionsParser::initTestCase()
{
    qInstallMessageHandler(UTILSLIB::ApplicationLogger::customLogWriter);
}

//=============================================================================================================

void TestCommandLineOptionsParser::cleanupTestCase()
{

}

//=============================================================================================================

void TestCommandLineOptionsParser::testCommandLineOptionType()
{
    auto option = CORELIB::CommandLineOptionType::withValue;
    QVERIFY(static_cast<int>(option) == 0);
    option = CORELIB::CommandLineOptionType::withoutValue;
    QVERIFY(static_cast<int>(option) == 1);

}

//=============================================================================================================
// MAIN
//=============================================================================================================

QTEST_GUILESS_MAIN(TestCommandLineOptionsParser)
//#include "test_commandlineoptionsparser.moc"
