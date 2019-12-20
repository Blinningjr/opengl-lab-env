#include "Reader.h"

#include <fstream>
#include <iostream>


namespace Simple3DGraphics {

    Reader::Reader() {

    }

    Reader::~Reader() {

    }


    /**
     *  Reads a file and returns its content.
    */
    char* Reader::readFile(char* filePath) {
        std::streampos size;
        char* fileText;
        std::ifstream file(filePath, std::ios::in|std::ios::binary|std::ios::ate);
        if (file.is_open())
        {
            size = file.tellg();
            fileText = new char[size];
            file.seekg (0, std::ios::beg);
            file.read (fileText, size);
            file.close();
        }
        else {
            std::cout << "Unable to open file";
            return NULL;
        }
        return fileText;
    }

}