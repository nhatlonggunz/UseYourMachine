#include "regexparser.h"
#include <stack>
#include <string>
#include <vector>

RegexParser::RegexParser()
{

}

std::vector<std::string> RegexParser::Tokenize(std::string prefix)
{
    std::vector<std::string> listTokens;
    std::stack<std::string> st;
    std::string delimiter = ",()";

    for(unsigned i = 0; i < prefix.size(); i++) {
        std::string token = "";

        // Read whole string until encountering a delimiter
        while(delimiter.find(prefix[i]) == std::string::npos &&
              i < prefix.size())
            token += prefix[i++];

        listTokens.push_back(token);
    }
    return listTokens;
}


