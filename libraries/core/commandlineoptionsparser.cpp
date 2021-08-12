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
#include <utility>

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
: m_bOptionsParsedCorrectly(false)
, m_bStopOnErrors(true)
{
    clear();
}

//=============================================================================================================

CommandLineOptionsParser::CommandLineOptionsParser(const CommandLineOption& opt)
: CommandLineOptionsParser()
{
    addOption(opt);
}

//=============================================================================================================

CommandLineOptionsParser::CommandLineOptionsParser(std::initializer_list<CommandLineOption> optList)
: CommandLineOptionsParser()
{
    addOptions(optList);
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
    m_options.push_back(opt);
}

//=============================================================================================================

void CommandLineOptionsParser::addOptions(std::initializer_list<CommandLineOption> optList)
{
    for( const auto& opt : optList)
    {
        m_options.push_back(opt);
    }
}

//=============================================================================================================

void CommandLineOptionsParser::parse(int argc, char** argv)
{
    for(int i = 1; i < argc; ++i)
    {
        std::string inputflag(argv[i]);
        auto search = flagSearch(inputflag);
        if(search.exists)
        {
            auto& opt = m_options[search.position];
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

CommandLineOptionsParser::searchResult CommandLineOptionsParser::optionSearch(const std::string& optName) const
{
    for (size_t i = 0; i < m_options.size(); ++i)
    {
        if(m_options[i].name == optName)
        {
            return searchResult(true, i);
        }
    }
    return searchResult(false, 0);
}

//=============================================================================================================

CommandLineOptionsParser::searchResult CommandLineOptionsParser::flagSearch(const std::string& inputFlag) const
{

    for(size_t i = 0; i < m_options.size(); ++i)
    {
        if(m_options[i].flagContained(inputFlag))
        {
            return searchResult(true, i);
        }
    }
    return searchResult(false,  0);
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
    searchResult result(optionSearch(optionName));
    if (result.exists)
    {
        return m_options[result.position].isSet;
    } else {
        return false;
    }
}

//=============================================================================================================

const std::string& CommandLineOptionsParser::value(const std::string& optionName) const
{
    return m_options[optionSearch(optionName).position].value;
}

//=============================================================================================================

std::string CommandLineOptionsParser::getHelpDescription() const
{

    size_t colWidth(getMaxSizeofFlagsString(6) + 5);

    std::string s("Options:\n");
    for(const auto& opt: m_options)
    {
        std::string flags(getFlagsAsString(opt));
        s += flags + std::string(colWidth - flags.size(),' ');

        for(auto it = opt.helpStr.begin(); it != opt.helpStr.end(); ++it)
        {
            if(it == opt.helpStr.begin())
            {
                s += *it + "\n";
            } else {
                s += std::string(colWidth, ' ') + *it + "\n";
            }
        }
    }
    s += "\n";
    return s;
}

//=============================================================================================================

std::string CommandLineOptionsParser::getFlagsAsString(const CommandLineOption& opt) const
{
    std::string concatFlags;
    for(auto flag = opt.flagsList.begin(); flag != opt.flagsList.end(); ++flag)
    {
        if (flag == opt.flagsList.begin())
        {
            concatFlags += *flag;
        } else {
            concatFlags += ", " + *flag;
        }

    }
    return concatFlags;
}

//=============================================================================================================

size_t CommandLineOptionsParser::getMaxSizeofFlagsString(size_t minSize) const
{
    size_t maxFlagStringSize(minSize);
    for(const auto& opt: m_options)
    {
        size_t fSize(getFlagsAsString(opt).size());
        if( maxFlagStringSize < fSize )
        {
            maxFlagStringSize = fSize;
        }
    }
    return maxFlagStringSize;
}
