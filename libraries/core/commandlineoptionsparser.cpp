//=============================================================================================================
/**
 * @file     commandlineoptionsparser.cpp
 * @author   Juan Garcia-Prieto <jgarciaprieto@mgh.harvard.edu>;
 * @since    0.1.9
 * @date     June, 2021
 *
 * @section  LICENSE
 *
 * Copyright (C) 2021, Juan Garcia-Prieto. All rights reserved.
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
 * @brief     CommandLineOptionsParser definition.
 *
 */
#include <iostream>
#include <functional>

#include "commandlineoptionsparser.h"

//=============================================================================================================
// INCLUDES
//=============================================================================================================

//=============================================================================================================
// USED NAMESPACES
//=============================================================================================================

using namespace CORELIB;

//=============================================================================================================
// DEFINE MEMBER METHODS
//=============================================================================================================

CommandLineOptionsParser::CommandLineOptionsParser()
: m_bOptionsParsedCorrectly(false),
m_bStopOnErrors(true)
{
    clear();
}

//=============================================================================================================

void CommandLineOptionsParser::clear()
{
    m_options.clear();
}

//=============================================================================================================

bool CommandLineOptionsParser::allOptionsParsedCorrectly() const
{
    return m_bOptionsParsedCorrectly;
}

//=============================================================================================================

void CommandLineOptionsParser::addOption(const CommandLineOption& opt)
{
    m_options[opt.name] = opt;
}

//=============================================================================================================

void CommandLineOptionsParser::parse(int argc, char** argv)
{
    for(int i = 1; i < argc; ++i)
    {
        std::cout << "Parsing input " << i << "\n";
        std::string inputflag(argv[i]);
        if(auto optionName = flagExists(inputflag))
        {
            auto opt = m_options[*optionName];
            opt.isSet = true;
            if (opt.type == CommandLineOptionType::withValue)
            {
                bool notLastInputArg(i < argc -1);
                if( notLastInputArg )
                {
                    opt.value = std::string(argv[i + 1]);
                    ++i;
                } else {
                    if(m_bStopOnErrors)
                    {
                        std::cout << "Error parsing input argumets. Arg: " << opt.name << " requires a following value.\n";
                        exit(1);
                    }
                }
            }
        } else {
            if(m_bStopOnErrors)
            {
                std::cout << "Error. Input argument unknown: " << argv[i] << "\n";
                exit(1);
            }
        }
    }
}

//=============================================================================================================

bool CommandLineOptionsParser::optionExists(const std::string& opt) const
{
    auto searchIt = m_options.find(opt);
    if (searchIt != m_options.end())
    {
        return true;
    } else {
        return false;
    }
}

//=============================================================================================================

std::optional<std::string> CommandLineOptionsParser::flagExists(const std::string& inputFlag)
{
    for (auto optsIt = m_options.begin(); optsIt != m_options.end(); ++optsIt)
    {
        if (optsIt->second.flagContained(inputFlag))
        {
            return std::optional<std::string>(optsIt->first);
        }
    }
    return {};
}

//=============================================================================================================

void CommandLineOptionsParser::setStopOnErrors(bool s)
{
    m_bStopOnErrors = s;
}

//=============================================================================================================

bool CommandLineOptionsParser::stopOnErrors() const
{
    return m_bStopOnErrors;
}

//=============================================================================================================

bool CommandLineOptionsParser::isSet(const std::string& optionName) const
{
    return m_options.at(optionName).isSet;
}

//=============================================================================================================

const std::string& CommandLineOptionsParser::value(const std::string& optionName) const
{
    return m_options.at(optionName).value;
}

