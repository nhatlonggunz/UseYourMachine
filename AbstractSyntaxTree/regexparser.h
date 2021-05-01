#ifndef REGEXPARSER_H
#define REGEXPARSER_H

#include <string>
#include <vector>
#include <string>

#include <AbstractSyntaxTree/abstractsyntaxtree.h>
#include "treenode.h"

class RegexParser
{
private:

    void ToAST(std::vector<std::string> listTokens);
    std::vector<std::string> Tokenize(std::string prefix);
public:
    RegexParser();

    void Parse(std::string regex);
};

#endif // REGEXPARSER_H
