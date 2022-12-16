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

    bool appendFile(String path, String& buf){
        if (path.charAt(0) != SLASH) path = String(SLASH) + path;
        File f = SPIFFS.open(path, "a+");

        if (!f) return false;

        uint32_t len = buf.length();

        for (uint32_t i = 0; i < len; i++) f.write(buf[i]);
        f.close();

        return true;
    }
    void checkFile(String path, String data) {
        if (path.charAt(0) != SLASH) path = String(SLASH) + path;

        if (!SPIFFS.exists(path)) writeFile(path, data);
    }

    bool removeLines(String path, int lineFrom, int lineTo) {
        int  c = 0;
        char tmp;

        if (path.charAt(0) != SLASH) path = String(SLASH) + path;

        String tmpPath = str(F_TMP) + path + str(F_COPY);

        File f  = SPIFFS.open(path, "r");
        File f2 = SPIFFS.open(tmpPath, "w");

        if (!f || !f2) return false;

        while (f.available()) {
            tmp = f.read();

            if ((c < lineFrom) || (c > lineTo)) f2.write(tmp);

            if (tmp == NEWLINE) c++;
        }

        f.close();
        f2.close();
        SPIFFS.remove(path);
        SPIFFS.rename(tmpPath, path);

        return true;
    }

    bool replaceLine(String path, int line, String& buf) {
        int  c = 0;
        char tmp;

        if (path.charAt(0) != SLASH) path = String(SLASH) + path;

        String tmpPath = "/tmp" + path + "_copy";

        File f  = SPIFFS.open(path, "r");
        File f2 = SPIFFS.open(tmpPath, "w");

        if (!f || !f2) return false;

        while (f.available()) {
            tmp = f.read();

            if (c != line) f2.write(tmp);
            else {
                f2.println(buf);

                while (f.read() != NEWLINE && f.available()) {}
                c++;
            }

            if (tmp == NEWLINE) c++;
        }

        f.close();
        f2.close();
        SPIFFS.remove(path);
        SPIFFS.rename(tmpPath, path);

        return true;
    }
}