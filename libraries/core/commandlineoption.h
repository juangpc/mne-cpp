//=============================================================================================================
/**
 * @file     commandlineoption.h
 * @author   Juan GPC <jgarciaprieto@mgh.harvard.edu>
 * @since    0.1.9
 * @date     June, 2021
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
 * @brief     CommandLineOption class declaration.
 *
 */
#ifndef COMMANDLINEOPTION_H
#define COMMANDLINEOPTION_H

#include "core_global.h"
#include <string>
#include <vector>

namespace CORELIB {

enum class CommandLineOptionType
{
    withValue,
    withoutValue
};

struct CORESHARED_EXPORT CommandLineOption
{
    CommandLineOption();

    CommandLineOption(const std::string& name,
                      const std::vector<std::string>& flags);

    CommandLineOption(const std::string& name,
                      const std::vector<std::string>& flags,
                      const std::string& helpLine);

    CommandLineOption(const std::string& name,
                  const std::vector<std::string>& flags,
                  const std::string& helpLine,
                  CommandLineOptionType type);

    CommandLineOption(std::string&& name,
                      std::vector<std::string>&& flags,
                      std::string&& helpLine,
                      CommandLineOptionType&& type);

    CommandLineOption(const CommandLineOption& opt);

    bool flagContained(const std::string& flag) const;

    std::string name;
    std::vector<std::string> flagsList;
    std::string helpStr;
    CommandLineOptionType type;
    bool isSet;
    std::string value;
};

} // namespace CORELIB

#endif // COMMANDLINEOPTION_H
