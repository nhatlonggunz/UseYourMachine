#ifndef REGEXPARSER_H
#define REGEXPARSER_H

#include <string>
#include <vector>
#include <string>

#include <AbstractSyntaxTree/abstractsyntaxtree.h>

class RegexParser
{
public:
    RegexParser();
    static std::vector<std::string> Tokenize(std::string prefix);
};

#endif // REGEXPARSER_H
