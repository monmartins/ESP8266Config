#include <ArduinoJson.h>

namespace file_spiffs {
    bool setupFILE();
    bool writeFile(String path, String& buf);
    bool readFile(String path, String& buf);
    String read(const char *path);
}