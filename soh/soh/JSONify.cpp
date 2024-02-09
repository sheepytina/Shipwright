// Name: JSONify for Shipwright
// Description: Text lookup/replacement. Localisation support for UIWidgets et al.

#include "JSONify.h"
#include <fstream>

namespace JSONify {

#ifdef JSONify_Logging
// Logging function
// Description
void JSONifyClass::SetLogging(bool foo) {
    logOutputToJson = foo;
}

// Logging function
// Output any text lookups to a log JSON, with the UID and the default string.
void JSONifyClass::LogOutput(const char* id, const char* defaultString) {
    if (logOutputToJson) {
        jsonOutput[id] = defaultString;

        std::ofstream outfile("JSONify_debug_output.json");
        outfile << std::setw(4) << jsonOutput << std::endl;
        outfile.close();
    }
}
#endif

// Load a JSON file by filename and keep the contents in memory.
void JSONifyClass::LoadFile(const char* foo) {

    // TODO: consider using filesystem here

    std::ifstream infile(foo);
    if (!infile) {
        return;
    }
    jsonData = json::parse(infile);
    infile.close();
}

// Look up string by ID. Returns a new string if one is found by that ID, otherwise returns default string.
const char* JSONifyClass::Replace(const char* id, const char* defaultString) {
#ifdef JSONify_Logging
    LogOutput(id, defaultString);
#endif
    if (!CVarGetInteger("gJSONify.Enabled", 0)) {
        return defaultString;
    }

    if (jsonData.contains(id)) {
        json n = jsonData[id];
        if (n.is_string() && !n.get<std::string>().empty()) {
            // std::string newString = n;
            // return newString.c_str();

            // const char* newString = std::string(n).c_str();
            //return std::string(n).c_str(); // TODO: that's a pretty serious warning i shouldn't ignore
            return std::string(n).c_str();
        }
    }
    return defaultString;
}

} // namespace JSONify
