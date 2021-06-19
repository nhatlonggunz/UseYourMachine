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

using namespace testing;



class TestAutomatonFixture:
        public testing::TestWithParam<std::string>
{

};

std::vector<std::string> getTestInputFiles(std::string subFolder) {

    char tmp[256];
    getcwd(tmp, 256);

    std::string filesPath(tmp);
    filesPath += "\\InputFiles";

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

INSTANTIATE_TEST_SUITE_P(SampleInputFiles,
                         TestAutomatonFixture,
                         testing::ValuesIn(getTestInputFiles("")));

TEST_P(TestAutomatonFixture, TestAutomaton) {
    std::string path = GetParam();

    std::ifstream fi(path);
    EXPECT_TRUE(fi.good());

    Parser parser;
    parser.ReadFromStream(fi);
    Automaton avtomat = parser.getAutomaton();

    // Validate test vector
    auto isDFA = parser.getTestIsDFA();
    auto isFinite = parser.getTestIsFinite();
    auto testWords = parser.getTestWords();

    EXPECT_NO_THROW(avtomat.ValidateTestVector(isDFA, isFinite, testWords));
}


#endif // TESTGENERAL_H
