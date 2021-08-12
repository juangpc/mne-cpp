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

//class CommandLineOption;

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
     * Class constructor already adding an option to the options list.
     *
     */
    CommandLineOptionsParser(const CommandLineOption& opt);

    //=========================================================================================================
    /**
     *
     * Class constructor which adds a list of options to the options list.
     *
     */
    CommandLineOptionsParser(std::initializer_list<CommandLineOption> optList);

    //=========================================================================================================
    /**
     *
     * @brief Check if all options have been correctly parsed.
     *
     */
    bool allOptionsParsedCorrectly() const;

    //=========================================================================================================
    /**
     *
     * Go through all the input arguments and parse the options previously added to the parser.
     *
     */
    void parse(int argc, char** argv);

    //=========================================================================================================
    /**
     *
     * Define the name, the characters or flags, to be identified in the command line by the user of the
     * application. And Specify a helpline of text to be included in a automatically generated help menu.
     * Specify the type of option {with value or without value (default)}.
     *
     */
    void addOption(const CommandLineOption& opt);

    //=========================================================================================================
    /**
     *
     * Define the name, the characters or flags, to be identified in the command line by the user of the
     * application. And Specify a helpline of text to be included in a automatically generated help menu.
     * Specify the type of option {with value or without value (default)}.
     *
     */
    void addOptions(std::initializer_list<CommandLineOption> optList);

    //=========================================================================================================
    /**
     *
     * Clear the parser of previously added options.
     *
     */
    void clear();

    //=========================================================================================================
    /**
     *
     * Specify whether the parser should stop the application when an unknown option has been used.
     *
     */
    void setStopOnErrors(bool s);

    //=========================================================================================================
    /**
     *
     * Retrieve if the current parser will stop on error or not.
     *
     */
    bool stopOnErrors() const;

    //=========================================================================================================
    /**
     *
     * Check whether an option has been introduced by the user.
     *
     */
    bool isSet(const std::string& opt) const;

    //=========================================================================================================
    /**
     *
     * Check the value of the option given by the user.
     *
     */
    const std::string& value(const std::string& opt) const;

    //=========================================================================================================
    /**
     *
     * Get a description text, minimally formatted, with all the command options, and their helper text.
     *
     */
    std::string getHelpDescription() const;

private:
    /**
     * This is a helper class to organize the results of searching in the vector.
     */
    struct searchResult
    {
        searchResult(bool b,
                     size_t pos)
        : exists(b)
        , position(pos)
        {}
        bool exists;
        size_t  position;
    };

    //=========================================================================================================
    /**
     *
     * Search the state of an option based on its Name.
     *
     */
    searchResult optionSearch(const std::string& optionName) const;

    //=========================================================================================================
    /**
     *
     * Search for an option based on one of its input flags.
     *
     */
    searchResult flagSearch(const std::string& inputFlag) const;

    //=========================================================================================================
    /**
     *
     * Get a description text, minimally formatted, with all the command options, and their helper text.
     *
     */
    std::string getFlagsAsString(const CommandLineOption& opt) const;

    //=========================================================================================================
    /**
     *
     * Helper function that returns the size of the longest flag string. This helps better format the code in the
     * helper text <getHelpDescription>"()".
     *
     */
    size_t getMaxSizeofFlagsString(size_t minSize) const;

    bool m_bOptionsParsedCorrectly;             /**< State variable to check if all options have been correctly parsed.>*/
    bool m_bStopOnErrors;                       /**< Stores if the parser will stop when finding an option not previously added.>*/
    std::vector<CommandLineOption> m_options;   /**< Container of the options to be parsed.>*/
};

} // CORELIB namespace

#endif // COMMANDLINEOPTIONSPARSER_H
