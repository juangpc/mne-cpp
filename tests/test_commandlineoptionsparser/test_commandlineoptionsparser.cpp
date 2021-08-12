
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

#include <iostream>

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
    void testCommandLineOptionConstructors();
    void testOptionsParserIsSet();
    void testOptionsParserValue();
    void testOptionsParserValue2();

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

void TestCommandLineOptionsParser::testCommandLineOptionConstructors()
{
    CORELIB::CommandLineOption option;
    QVERIFY(option.name.empty());
    QVERIFY(option.flagsList.size() == 0);
    QVERIFY(option.helpStr.empty());
    QVERIFY(option.type == CORELIB::CommandLineOptionType::withoutValue);
    QVERIFY(option.isSet == false);
    QVERIFY(option.value == std::string());

    CORELIB::CommandLineOption option2("help",{"-h","--h","--help","/h","/help"}, {"Display this help.", "this is the second line"});
    QVERIFY(option2.name == "help");
    QVERIFY(option2.flagsList.size() == 5);
    QVERIFY((option2.helpStr.size() == 2) && (option2.helpStr[0] == "Display this help."));
    QVERIFY(option2.type == CORELIB::CommandLineOptionType::withoutValue);
    QVERIFY(option2.isSet == false);
    QVERIFY(option2.value == std::string());
    QVERIFY(!option2.flagContained("-v"));
    QVERIFY(option2.flagContained("--h"));
    QVERIFY(option2.flagContained("/h"));
}

//=============================================================================================================

void TestCommandLineOptionsParser::testOptionsParserIsSet()
{
    CORELIB::CommandLineOption option1("help",{"-h","--h","--help","/h","/help"}, {"Display this help.", "this is the second line"});
    CORELIB::CommandLineOption option2("input",{"-i","--input"}, {"Input file.","This is a second line with a lot of info."}, CORELIB::CommandLineOptionType::withValue);
    CORELIB::CommandLineOption option3("newOne",{"~wow","~w"},{"This is when your wowed","The following line"});
    auto list = {option1, option2, option3};

    auto argc(3);
    char* argv[] = {const_cast<char*> ("bla"),
                    const_cast<char*> ("--input"),
                    const_cast<char*> ("blu.txt")};
    CORELIB::CommandLineOptionsParser parser(list);
    parser.parse(argc, argv );

    QVERIFY(parser.isSet("input"));
    QVERIFY(!parser.isSet("help"));
    QVERIFY(!parser.isSet("newOne"));
}

//=============================================================================================================

void TestCommandLineOptionsParser::testOptionsParserValue()
{
    CORELIB::CommandLineOption option1("help",{"-h","--h","--help","/h","/help"}, {"Display this help.", "this is the second line"});
    CORELIB::CommandLineOption option2("input",{"-i","--input"}, {"Input file.","This is a second line with a lot of info."}, CORELIB::CommandLineOptionType::withValue);
    CORELIB::CommandLineOption option3("newOne",{"~wow","~w"},{"This is when your wowed","The following line"});
    auto list = {option1, option2, option3};

    auto argc(3);
    char* argv[] = {const_cast<char*> ("bla"),
                    const_cast<char*> ("--input"),
                    const_cast<char*> ("blu.txt")};
    CORELIB::CommandLineOptionsParser parser(list);
    parser.parse(argc, argv );

    QVERIFY(parser.isSet("input"));
    QVERIFY(parser.value("input") == std::string("blu.txt"));
}


void TestCommandLineOptionsParser::testOptionsParserValue2()
{
    CORELIB::CommandLineOption option1("help",{"-h","--h","--help","/h","/help"}, {"Display this help.", "this is the second line"});
    CORELIB::CommandLineOption option2("input",{"-i","--input"}, {"Input file.","This is a second line with a lot of info."}, CORELIB::CommandLineOptionType::withValue);
    CORELIB::CommandLineOption option3("newOne",{"~wow","~w"},{"This is when your wowed","The following line"});

    auto argc(4);
    char* argv[] = {const_cast<char*> ("bla"),
                    const_cast<char*> ("--input"),
                    const_cast<char*> ("blu.txt"),
                    const_cast<char*> ("~w")};

    CORELIB::CommandLineOptionsParser parser;
    parser.addOption(option1);
    parser.addOption(option2);
    parser.addOption(option3);
    parser.parse(argc, argv);

    QVERIFY(!parser.isSet("ioqusa"));
    QVERIFY(parser.isSet("input"));
    QVERIFY(parser.value("input") == "blu.txt");

    QVERIFY(parser.isSet("newOne"));
    QVERIFY(parser.value("newOne") == "");
}

//=============================================================================================================
// MAIN
//=============================================================================================================

QTEST_GUILESS_MAIN(TestCommandLineOptionsParser)
#include "test_commandlineoptionsparser.moc"

