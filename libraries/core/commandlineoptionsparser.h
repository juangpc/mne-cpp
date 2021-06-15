//=============================================================================================================
/**
 * @file     commandlineoptionsparser.h
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
 * @brief     CommandLineOptionsParser class declaration.
 *
 */
#ifndef COMMANDLINEOPTIONSPARSER_H
#define COMMANDLINEOPTIONSPARSER_H

//=============================================================================================================
// INCLUDES
//=============================================================================================================
#include "core_global.h"
#include <string>
#include <vector>
#include <map>
#include <optional>

#include "commandlineoption.h"

//=============================================================================================================
// FORWARD DECLARATIONS
//=============================================================================================================

//=============================================================================================================
// NAMESPACE CORELIB
//=============================================================================================================

namespace CORELIB {

//=============================================================================================================
// FORWARD DECLARATIONS
//=============================================================================================================



struct CommandLineOption;

//=============================================================================================================
/**
 * CommandLineParser class is a tool to parse through the command line arguments provided during the application
 * call.
 *
 * @brief Command-line options parser.
 */
class CORESHARED_EXPORT CommandLineOptionsParser
{
public:

    /**
     *
     * @brief Default class contstructor.
     *
     */
    CommandLineOptionsParser();

    //=========================================================================================================
    /**
     *
     * @brief Check if all options have been correctly parsed.
     *
     */
    bool allOptionsParsedCorrectly() const;

    void parse(int argc, char** argv);

    void addOption(const CommandLineOption& opt);

    void clear();

    void setStopOnErrors(bool s);

    bool stopOnErrors() const;

    bool isSet(const std::string& opt) const;
    const std::string& value(const std::string& opt) const;

private:

    bool optionExists(const std::string& optionName) const;
    std::optional<std::string> flagExists(const std::string& inputFlag);

    bool m_bOptionsParsedCorrectly;             /**<State variable to check if all options have been correctly parsed>.*/
    bool m_bStopOnErrors;
    std::map<std::string, CommandLineOption> m_options;
};

} // CORELIB namespace

#endif // COMMANDLINEOPTIONSPARSER_H
