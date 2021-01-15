#ifndef jsondb
#define jsondb

#include <fstream>
#include <iostream>
#include <string>

class JsonDB {
   private:
    std::string file_location;

   public:
    JsonDB();

    JsonDB(std::string location = "") {
        file_location = location;
    }

    void setLocation(std::string location) {
        file_location = location;
    }

    bool fileExists() {
        std::ifstream file(file_location);
        return file.good();
    }

    std::string readFile(std::string defaultEntry = "") {
        std::string fileContent, lineContent;
        std::ifstream file(file_location);

        if (!file.good())
            return defaultEntry;

        while (std::getline(file, lineContent)) {
            fileContent.append(lineContent + "\n");
        }

        file.close();

        return fileContent;
    }

    void writeFile(std::string content) {
        std::ofstream file(file_location);
        file << content;
        file.close();
    }
};

#endif