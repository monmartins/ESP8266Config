#include "file.h"
#include "functions.h"
#include <FS.h>

namespace file_spiffs {
    bool setupFILE() {
        bool success = SPIFFS.begin();
        if (!success) {
            Serial.println("Error mounting the file system");
        }
        return success;
    }

//   String input;
//   input="Test buf and input";
//   file_spiffs::writeFile("/file.txt",input);
    bool writeFile(String path, String& buf) {
        if (path.charAt(0) != SLASH) path = String(SLASH) + path;
        File f = SPIFFS.open(path, "w+");

        if (!f) return false;

        uint32_t len = buf.length();

        for (uint32_t i = 0; i < len; i++) f.write(buf.charAt(i));
        f.close();

        return true;
    }
//   String output;
//   file_spiffs::readFile("/file.txt",output);
//   Serial.println("File Content:");
//   Serial.println(output);
    bool readFile(String path, String& buf) {
        if (path.charAt(0) != SLASH) path = String(SLASH) + path;
        File f = SPIFFS.open(path, "r");

        if (!f) return false;

        if (f.size() == 0) return false;

        while (f.available()) buf += (char)f.read();

        f.close();

        return true;
    }
}