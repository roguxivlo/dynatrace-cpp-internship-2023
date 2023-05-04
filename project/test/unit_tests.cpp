#define BOOST_TEST_MODULE My Test
#include <boost/test/included/unit_test.hpp>
#include <fstream>
#include <exception>
#include <nlohmann/json.hpp>
#include <iostream>

#include "../src/internship.h"

// To run:
// sudo docker run --rm -w /usr/src/internship_task -v "$(pwd):/usr/src/internship_task" internship_task:latest

std::string jsonFileName = "test.json";
std::string invalidFileName = "invalid.json";

using json = nlohmann::json;

json jsonString = {{"name", true}, {"os", true}, {"versions", {{"cycle", "17"}, {"lts", true}, {"releaseDate", "2021-10-18"}, {"eol", "2021-10-30"}}}};
json invalidOS = {{"name", true}, {"os", 123}, {"versions", {{"cycle", "17"}, {"lts", true}, {"releaseDate", "2021-10-18"}, {"eol", "2021-10-30"}}}};


BOOST_AUTO_TEST_CASE(invalid_count)
{
  int count = -1;
  std::ofstream file(jsonFileName);

  try {
    file << jsonString;
    file.close();
    internship::solution(jsonFileName, count);
  } catch (const std::invalid_argument& e) {
    BOOST_TEST_MESSAGE(true);
  }
  BOOST_TEST_MESSAGE(false);
};

BOOST_AUTO_TEST_CASE(invalid_json_file_name)
{
  int count = 1;
  std::ofstream file(jsonFileName);

  try {
    file << jsonString;
    file.close();
    internship::solution(invalidFileName, count);
  } catch (const std::invalid_argument& e) {
    BOOST_TEST_MESSAGE(true);
  }
  BOOST_TEST_MESSAGE(false);
}

BOOST_AUTO_TEST_CASE(invalid_attribute_os)
{
    int count = 1;
    std::ofstream file(jsonFileName);
    
    try {
        file << invalidOS;
        file.close();
        internship::solution(jsonFileName, count);
    } catch (const std::exception& e) {
        BOOST_TEST_MESSAGE(false); // should not throw
    }
    BOOST_TEST_MESSAGE(true);
}

BOOST_AUTO_TEST_CASE(invalid_attribute_cycle)
{
    // Should produce only 1 line of output;
    int count = 2;
    json invalidCycle = {{"name", true}, {"os", 123}, {"versions", {{"cycle", "17"}, {"lts", true}, {"releaseDate", "2021-10-18"}, {"eol", "2021-10-30"}}, {{"cycle", false}, {"lts", true}, {"releaseDate", "2021-10-18"}, {"eol", "2027-10-30"}}}};
    try{
        std::ofstream file(jsonFileName);
        file << invalidCycle;
        file.close();
        internship::solution(jsonFileName, count);
    } catch (const std::exception& e) {
        BOOST_TEST_MESSAGE(false); // should not throw
    }
    BOOST_TEST_MESSAGE(true);
}

BOOST_AUTO_TEST_CASE(invalid_attribute_release_date)
{
    // Should produce only 1 line of output;
    int count = 2;
    json invalidCycle = {{"name", true}, {"os", 123}, {"versions", {{"cycle", "17"}, {"lts", true}, {"releaseDate", "2021-10-18"}, {"eol", "2021-10-30"}}, {{"cycle", "23"}, {"lts", true}, {"releaseDate", "2-10-18"}, {"eol", "2027-10-30"}}}};
    try{
        std::ofstream file(jsonFileName);
        file << invalidCycle;
        file.close();
        internship::solution(jsonFileName, count);
    } catch (const std::exception& e) {
        BOOST_TEST_MESSAGE(false); // should not throw
    }
    BOOST_TEST_MESSAGE(true);
}

BOOST_AUTO_TEST_CASE(invalid_attribute_eol)
{
    // Should produce only 1 line of output;
    int count = 2;
    json invalidCycle = {{"name", true}, {"os", 123}, {"versions", {{"cycle", "17"}, {"lts", true}, {"releaseDate", "2021-10-18"}, {"eol", 125}}, {{"cycle", "23"}, {"lts", true}, {"releaseDate", "2-10-18"}, {"eol", "2027-10-30"}}}};
    try{
        std::ofstream file(jsonFileName);
        file << invalidCycle;
        file.close();
        internship::solution(jsonFileName, count);
    } catch (const std::exception& e) {
        BOOST_TEST_MESSAGE(false); // should not throw
    }
    BOOST_TEST_MESSAGE(true);
}

BOOST_AUTO_TEST_CASE(support_length_negative)
{
    // Should produce only 1 line of output;
    int count = 2;
    json invalidCycle = {{"name", true}, {"os", 123}, {"versions", {{"cycle", "17"}, {"lts", true}, {"releaseDate", "2021-10-18"}, {"eol", "2010-10-18"}}, {{"cycle", "23"}, {"lts", true}, {"releaseDate", "2-10-18"}, {"eol", "2027-10-30"}}}};
    try{
        std::ofstream file(jsonFileName);
        file << invalidCycle;
        file.close();
        internship::solution(jsonFileName, count);
    } catch (const std::exception& e) {
        BOOST_TEST_MESSAGE(false); // should not throw
    }
    BOOST_TEST_MESSAGE(true);
}