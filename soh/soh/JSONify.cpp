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

// Start of key functionality

// Read in a JSON file by filename.
void JSONifyClass::LoadFile(const char* foo) {
    std::ifstream infile(foo);
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
    
    return defaultString; // do other things
}

// End of key functionality

} // namespace JSONify
