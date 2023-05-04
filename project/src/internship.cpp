#include "internship.h"

#include <chrono>
#include <exception>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <set>
#include <typeinfo>

#include "boost/date_time/gregorian/gregorian.hpp"

namespace {
using json = nlohmann::json;
using namespace boost::gregorian;


typedef int days_t;
// (OS, Cycle)
typedef std::pair<std::string, std::string> osVersion_t;


class VersionSupportLength {
 public:
  VersionSupportLength(const json& attributes, const std::string& name) {
    if (attributes.at("cycle").is_string() &&
        attributes.at("releaseDate").is_string() &&
        attributes.at("eol").is_string()) {
      osVersion = std::make_pair(name, attributes.at("cycle"));
      supportLength = (from_string(attributes.at("eol")) -
                       from_string(attributes.at("releaseDate")))
                          .days() +
                      1;  // +1 because the last day is included
    } else {
      throw std::invalid_argument("Invalid attributes");
    }
    if (supportLength < 0) {
      throw std::invalid_argument("Invalid attributes");
    }
  }


  void print() const {
    std::cout << osVersion.first << " " << osVersion.second << " "
              << supportLength;
    std::cout << std::endl;
  }

  
  bool operator<(const VersionSupportLength& other) const {
    if (supportLength == other.supportLength) {
      if (osVersion.first == other.osVersion.first) {
        return osVersion.second < other.osVersion.second;
      }
      return osVersion.first < other.osVersion.first;
    }
    return supportLength < other.supportLength;
  }

  
  bool operator>(const VersionSupportLength& other) const {
    if (supportLength == other.supportLength) {
      if (osVersion.first == other.osVersion.first) {
        return osVersion.second < other.osVersion.second;
      }
      return osVersion.first > other.osVersion.first;
    }
    return supportLength > other.supportLength;
  }

  
  osVersion_t osVersion;
  days_t supportLength;
};
}  // namespace



namespace internship {


// do not remove this function
void solution(const std::string& jsonFileName, int elementsCount) {
  if (elementsCount < 0) {
    throw std::invalid_argument("elementsCount must be positive");
  }

  try {
    std::ifstream f(jsonFileName);
    json data = json::parse(f);
  

    auto cmp = [](const VersionSupportLength& a, const VersionSupportLength& b) {
        return a > b;
    };
    std::set<VersionSupportLength, decltype(cmp)> osVersionsRanking(cmp);

    // Iterate through products.
    // Products which have invalide attributes are ignored.
    for (const auto& [id, product] : data.items()) {
        if (product["os"] == true) {
        // Iterate through product versions:
        // If no attribute named "versions" is found, the product is ignored.
        const auto& versions = product["versions"];
        for (const auto& [versionId, attributes] : versions.items()) {
            // Check if version attributes: "cycle", "releaseDate", "eol" exist
            // and are of type string. If not, the version is ignored.
            try {
            VersionSupportLength versionSupportLength(
                attributes, product["name"].get<std::string>());

            auto res = osVersionsRanking.emplace(versionSupportLength);

            // If the ranking is full, remove the last element.
            if (osVersionsRanking.size() > elementsCount) {
                osVersionsRanking.erase(std::prev(osVersionsRanking.end()));
            }
            } catch (const std::exception& e) {
                // if version attributes are not valid, ignore the version.
                continue;
            }
        }
        }
    }

    // Print the ranking.
    for (const auto& versionSupportLength : osVersionsRanking) {
        versionSupportLength.print();
    }
  } catch (const std::exception& e) {
    throw std::invalid_argument("Invalid json file name");
  }
}
}  // namespace internship