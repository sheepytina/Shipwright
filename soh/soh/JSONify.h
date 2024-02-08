#pragma once
#include <libultraship/libultraship.h>
#include <nlohmann/json.hpp>

#define JSONify_Logging

namespace JSONify {

using json = nlohmann::ordered_json;

class JSONifyClass {
  private:
    // important stuff
    json jsonData;

#ifdef JSONify_Logging
    // logging
    json jsonOutput;
    bool logOutputToJson = true;
#endif

  public:
#ifdef JSONify_Logging
    // logging functions
    void SetLogging(bool foo);
    void LogOutput(const char* id, const char* defaultString);
#endif

    // key functionality
    void LoadFile(const char* filename);
    const char* Replace(const char* id, const char* defaultString);
};
} // namespace JSONify
