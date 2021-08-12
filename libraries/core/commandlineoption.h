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

//=============================================================================================================
// INCLUDES
//=============================================================================================================

#include "core_global.h"

#include <string>
#include <vector>

//=============================================================================================================
// USED NAMESPACES
//=============================================================================================================

namespace CORELIB {
/**
 * Enumerator class to store if the CommandLineOption is of type with or without value.
 */
enum class CommandLineOptionType
{
    withValue,
    withoutValue
};

/**
 * This class specifies an option which could be set by the user in an applicaiton and then parsed by the
 * CommandLineOptionPasrser.
 *
 * @brief One possible option to be set by the user.
 */
class CORESHARED_EXPORT CommandLineOption
{
public:
    //=========================================================================================================
    /**
     *
     * Default constructor. Creates an empty object.
     *
     */
    CommandLineOption();

    //=========================================================================================================
    /**
     *
     * Constructor of an option, given its name, the flags that will set it, the helper line and wether it will be
     * followed by a value or not.
     *
     */
    CommandLineOption(const std::string& name,
                      const std::vector<std::string>& flags);

    //=========================================================================================================
    /**
     *
     * Constructor of an option, given its name, the flags that will set it, the helper line and wether it will be
     * followed by a value or not.
     *
     */
    CommandLineOption(const std::string& name,
                      const std::vector<std::string>& flags,
                      const std::vector<std::string>& helpLine);

    //=========================================================================================================
    /**
     *
     * Constructor of an option, given its name, the flags that will set it, the helper line and wether it will be
     * followed by a value or not.
     *
     */
    CommandLineOption(const std::string& name,
                      const std::vector<std::string>& flags,
                      const std::vector<std::string>& helpLine,
                      CommandLineOptionType type);

    //=========================================================================================================
    /**
     *
     * Copy an option from anotherone.
     *
     */
    CommandLineOption(const CommandLineOption& opt);

    //=========================================================================================================
    /**
     *
     * Check if a given flag will set this option.
     *
     */
    bool flagContained(const std::string& flag) const;

    std::string name;                   /**< Name of the option. Serves the developer using the option and it will show in the help.>*/
    std::vector<std::string> flagsList; /**< List of flags to be used in an application call.>*/
    std::vector<std::string> helpStr;   /**< Help string for this specific option. It is used to build a Help Description menu.>*/
    CommandLineOptionType type;         /**< Type of this option. CommandLineOptionType.>*/
    bool isSet;                         /**< Bool value specifying if the parser has already identified this option as set/used by the user.>*/
    std::string value;                  /**< If the option has value, the value given by the user will be stored here.>*/
};

} // namespace CORELIB

#endif // COMMANDLINEOPTION_H
