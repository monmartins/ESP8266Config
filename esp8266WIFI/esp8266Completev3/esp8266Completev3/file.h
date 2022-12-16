#include <ArduinoJson.h>

namespace file_spiffs {
    bool setupFILE();
    bool writeFile(String path, String& buf);
    bool readFile(String path, String& buf);
    String read(const char *path);
    bool appendFile(String path, String& buf);
    void checkFile(String path, String data);
    bool removeLines(String path, int lineFrom, int lineTo);
    bool replaceLine(String path, int line, String& buf);
}