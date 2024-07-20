#include <gtest/gtest.h>
#include "counters.h"
#include <QString>
#include <string>
#include <algorithm>
#include <sstream>

const QString NO_ERRORS_DIR = "data_raw_32_rand_ch_offs";
const QString WITH_ERRORS_DIR = "data_raw_32_rand_ch_offs_break";

TEST(CountersTest, NoErrors) {
    std::string result = processDirectory(NO_ERRORS_DIR);

    int lineCount = std::count(result.begin(), result.end(), '\n');
    if (lineCount != 8) {
        FAIL() << "Expected exactly 8 lines in the output, but got " << lineCount;
    }

    std::string expected =
        "File: afe0_core0.pcm, Total errors: 0\n"
        "File: afe0_core1.pcm, Total errors: 0\n"
        "File: afe0_core2.pcm, Total errors: 0\n"
        "File: afe0_core3.pcm, Total errors: 0\n"
        "File: afe0_core4.pcm, Total errors: 0\n"
        "File: afe0_core5.pcm, Total errors: 0\n"
        "File: afe0_core6.pcm, Total errors: 0\n"
        "File: afe0_core7.pcm, Total errors: 0\n";

    if (result != expected) {
        FAIL() << "The result string doesn't match the expected string\n"
               << "Expected:\n" << expected
               << "Actual:\n" << result;
    }
}

TEST(CountersTest, WithErrors) {
    std::string result = processDirectory(WITH_ERRORS_DIR);
    if (result.empty()) {
        FAIL() << "The result string is empty";
    }

    int lineCount = std::count(result.begin(), result.end(), '\n');
    if (lineCount <= 8) {
        FAIL() << "Expected more than 8 lines in the output, but got " << lineCount;
    }

    if (result.find("Expected:") == std::string::npos) {
        FAIL() << "No error lines found in the result";
    }

    // Дополнительная проверка на наличие словосочетания "Total errors: 128" ровно 8 раз
    std::string expectedSubstring = "Total errors: 128";
    int substringCount = 0;
    std::string::size_type pos = 0;
    while ((pos = result.find(expectedSubstring, pos)) != std::string::npos) {
        ++substringCount;
        pos += expectedSubstring.length();
    }
    int expectedSubstringCount = 8;

    if (substringCount != expectedSubstringCount) {
        FAIL() << "Expected substring '" << expectedSubstring << "' to occur "
               << expectedSubstringCount << " times, but it occurred " << substringCount << " times";
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
