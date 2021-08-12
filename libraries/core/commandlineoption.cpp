//=============================================================================================================
/**
 * @file     commandlineoptions.cpp
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
 * @brief     CommandLineOption definition.
 *
 */

#include <utility>
#include <iostream>

//=============================================================================================================

#include "commandlineoption.h"

//=============================================================================================================

using namespace CORELIB;

//=============================================================================================================

CommandLineOption::CommandLineOption()
: CommandLineOption({}, {}, {}, CommandLineOptionType::withoutValue)
{
}

//=============================================================================================================

CommandLineOption::CommandLineOption(const std::string& name,
                                     const std::vector<std::string>& flags)
: CommandLineOption(name, flags, {}, CommandLineOptionType::withoutValue)
{
}

//=============================================================================================================

CommandLineOption::CommandLineOption(const std::string& name,
                  const std::vector<std::string>& flags,
                  const std::vector<std::string>& helpLine)
: CommandLineOption(name, flags, helpLine, CommandLineOptionType::withoutValue)
{
}

//=============================================================================================================

CommandLineOption::CommandLineOption(const std::string& name,
                  const std::vector<std::string>& flags,
                  const std::vector<std::string>& helpLine,
                  CommandLineOptionType type)
: name(name)
, flagsList(flags)
, helpStr(helpLine)
, type(type)
, isSet(false)
, value("")
{
}

//=============================================================================================================

CommandLineOption::CommandLineOption(const CommandLineOption& opt)
: name(opt.name)
, flagsList(opt.flagsList)
, helpStr(opt.helpStr)
, type(opt.type)
, isSet(opt.isSet)
, value(opt.value)
{
}

//=============================================================================================================

bool CommandLineOption::flagContained(const std::string& flag) const
{
    for (const auto& s: flagsList)
    {
        if (s == flag)
        {
            return true;
        }
    }
    return false;
}
