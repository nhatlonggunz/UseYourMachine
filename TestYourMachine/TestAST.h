#ifndef TESTAST_H
#define TESTAST_H

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include "AbstractSyntaxTree/abstractsyntaxtree.h"

using namespace testing;

struct RegexTestData
{
    std::string prefixRegex;
    std::string infixRegex;
    bool isLanguageFinite;
    std::vector<std::string> language;
};

class BasicRegexTestFixture:
        public testing::TestWithParam<RegexTestData>
{

};

std::vector<RegexTestData> listInfiniteRegex = {
    RegexTestData {"*(|(.(a,b), c))", "(((a.b)|c))*", false, {}},
    RegexTestData {"*(a)", "(a)*", false, {}},
    RegexTestData {"|(.(.(|(*(a),b),*(c)), *(d)), .(|(e,a), *(c)))",
                   "(((((a)*|b).(c)*).(d)*)|((e|a).(c)*))",
                   false, {}},
    RegexTestData {"|(. (.(| (a, b)  , *(c)   ), d ),    .(|(e,a),             c))",
                   "((((a|b).(c)*).d)|((e|a).c))",
                   false, {}},
};
INSTANTIATE_TEST_SUITE_P(InfiniteRegex,
                         BasicRegexTestFixture,
                         testing::ValuesIn(listInfiniteRegex));

std::vector<RegexTestData> listFiniteRegex = {
    RegexTestData {"|(.(a,b), c))", "((a.b)|c)", true, {"ab", "c"}},
    RegexTestData {"|(.(a,|(a,b)), c))", "((a.(a|b))|c)", true, {"aa", "ab", "c"}},
    RegexTestData {"*(_)", "(_)*", true, {""}},
//    RegexTestData {"|(.(.(|(*(a),b),*(c)), *(d)), .(|(e,a), *(c)))",
//                   "(((((a)*|b).(c)*).(d)*)|((e|a).(c)*))",
//                   true, {}},
    RegexTestData {"|(. (.(| (a, b)  , c   ), d ),    .(|(e,a),             c))",
                   "((((a|b).c).d)|((e|a).c))",
                   true, {"acd", "bcd", "ec", "ac"}},
};
INSTANTIATE_TEST_SUITE_P(FiniteRegex,
                         BasicRegexTestFixture,
                         testing::ValuesIn(listFiniteRegex));

TEST_P(BasicRegexTestFixture, basicRegex) {
    RegexTestData testData = GetParam();

    AbstractSyntaxTree ast(testData.prefixRegex);
    EXPECT_EQ(ast.toString(), testData.infixRegex);

    std::vector<std::string> language;
    FiniteStateAutomaton avtomat = ast.toNFA();
    bool isLanguageFinite = avtomat.ListAllWords(language);

    EXPECT_EQ(testData.isLanguageFinite, isLanguageFinite);
    EXPECT_THAT(testData.language, testing::UnorderedElementsAreArray(language));
}

#endif // TESTAST_H
