#ifndef TESTGENERAL_H
#define TESTGENERAL_H

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include "AbstractSyntaxTree/abstractsyntaxtree.h"
#include "Automata/parser.h"

#include <fstream>

#include <dirent.h>
#include <vector>
#include <cstring>

#include <typeinfo>

using namespace testing;


std::vector<std::string> getTestInputFiles(std::string subFolder) {

    char tmp[256];
    getcwd(tmp, 256);

    std::string filesPath(tmp);
    filesPath += "\\InputFiles" + subFolder;

    DIR *dir; struct dirent *diread;
    std::vector<std::string> files;

    if ((dir = opendir(filesPath.c_str())) != nullptr) {
        while ((diread = readdir(dir)) != nullptr) {
            if(strcmp(diread->d_name, ".") == 0 || strcmp(diread->d_name, "..") == 0)
                continue;
            files.push_back(filesPath + "\\" + diread->d_name);
        }
        closedir (dir);
    } else {
        throw std::invalid_argument("list all files messed up");
    }

    return files;
}

class TestDFAFixture:
        public testing::TestWithParam<std::string>
{

};

INSTANTIATE_TEST_SUITE_P(SampleInputFiles,
                         TestDFAFixture,
                         testing::ValuesIn(getTestInputFiles("\\DFA")));

TEST_P(TestDFAFixture, TestDFA) {
    std::string path = GetParam();

    std::ifstream fi(path);
    EXPECT_TRUE(fi.good());

    Parser parser;
    parser.ReadFromStream(fi);
    Automaton *avtomat = parser.getNewAvtomat();

    // Validate test vector
    auto isDFA = parser.getTestIsDFA();
    auto isFinite = parser.getTestIsFinite();
    auto testWords = parser.getTestWords();

    EXPECT_NO_THROW(avtomat->ValidateTestVector(isDFA, isFinite, testWords));

    delete avtomat;
}


/* test NFA */
class TestNFAFixture:
        public testing::TestWithParam<std::string>
{

};

INSTANTIATE_TEST_SUITE_P(SampleInputFiles,
                         TestNFAFixture,
                         testing::ValuesIn(getTestInputFiles("\\NFA")));

TEST_P(TestNFAFixture, TestNFA) {
    std::string path = GetParam();

    std::ifstream fi(path);
    EXPECT_TRUE(fi.good());

    Parser parser;
    parser.ReadFromStream(fi);
    Automaton *avtomat = parser.getNewAvtomat();

    // Validate test vector
    auto isDFA = parser.getTestIsDFA();
    auto isFinite = parser.getTestIsFinite();
    auto testWords = parser.getTestWords();

    EXPECT_NO_THROW(avtomat->ValidateTestVector(isDFA, isFinite, testWords));

    delete avtomat;
}

/* test PDA */
class TestPDAFixture:
        public testing::TestWithParam<std::string>
{

};

INSTANTIATE_TEST_SUITE_P(SampleInputFiles,
                         TestPDAFixture,
                         testing::ValuesIn(getTestInputFiles("\\PDA")));

TEST_P(TestPDAFixture, TestPDA) {
    std::string path = GetParam();

    std::ifstream fi(path);
    EXPECT_TRUE(fi.good());

    Parser parser;
    parser.ReadFromStream(fi);
    Automaton* avtomat = parser.getNewAvtomat();

    PushDownAutomaton* pdtmp = dynamic_cast<PushDownAutomaton*>(avtomat);

    EXPECT_FALSE(pdtmp == nullptr);

    auto testWords = parser.getTestWords();

    EXPECT_NO_THROW(pdtmp->ValidateTestVector(0, 0, testWords));

    delete avtomat;
}


#endif // TESTGENERAL_H
