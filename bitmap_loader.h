//
// Created by DamianPC on 6/25/2020.
//

#ifndef UNTITLED_BITMAP_LOADER_H
#define UNTITLED_BITMAP_LOADER_H

#include <string>
#include <cmath>
#include <iostream>
#include <fstream>
#include <memory>

const unsigned int BMP_HEADER_LENGTH = 14;

class Bitmap{
public:
    unsigned int size;
    unsigned int width;
    unsigned int height;
    unsigned int rawDataSize;
    char * data;

    ~Bitmap(){
        if (data != nullptr){
            delete data;
        }
    }
};

class DIBParser{
public:
    static void parseBITMAPINFOHEADER(Bitmap & bitmap, std::string filename);
};

class BitmapLoader{
public:
    void load(Bitmap & bitmap, std::string filename);
    void processHeader(Bitmap & bitmap);
    void processDIBHeader(Bitmap & bitmap);
    void processBody(Bitmap & bitmap);

    static void readBytes(std::string filename, char * buffer, unsigned int n, unsigned int startpos = 0);
    template<typename T>
    static T hexConvert(std::string bytes);
private:
    std::string filename;
    unsigned int dibHeaderLength;

    DIBParser dibParser;
};


void BitmapLoader::load(Bitmap & bitmap, std::string filename) {
    this->filename = filename;
    try{
        processHeader(bitmap);
        processDIBHeader(bitmap);
        processBody(bitmap);
    }
    catch(std::runtime_error e){
        std::cerr << e.what() << std::endl;
        return;
    }
}

void BitmapLoader::processHeader(Bitmap & bitmap) {
    char buffer[BMP_HEADER_LENGTH];
    readBytes(this->filename, buffer, BMP_HEADER_LENGTH);
    std::string s(&buffer[0], &buffer[BMP_HEADER_LENGTH]);
    bitmap.size = hexConvert<unsigned int>(s.substr(2, 4));
}

void BitmapLoader::processDIBHeader(Bitmap & bitmap) {
    // read first 4 bytes of DIB header to determine its type
    const unsigned int DIB_HEADER_OFFSET = 14;
    char firstChar[4];
    readBytes(this->filename, firstChar, 4, DIB_HEADER_OFFSET);
    std::string s(&firstChar[0], &firstChar[4]);
    dibHeaderLength = hexConvert<unsigned int>(s);

    switch(dibHeaderLength){
        case 40: dibParser.parseBITMAPINFOHEADER(bitmap, this->filename); break;
        default: throw std::runtime_error("DIB Header not supported!");
    }
}

void BitmapLoader::processBody(Bitmap &bitmap) {
    const unsigned int PIXEL_DATA_OFFSET = BMP_HEADER_LENGTH + dibHeaderLength;
    char buffer[bitmap.rawDataSize];
    readBytes(this->filename, buffer, bitmap.rawDataSize, PIXEL_DATA_OFFSET);

    unsigned int byteNumber = 0;
    for (unsigned int cols = 0; cols < bitmap.width; cols++){
        for (unsigned int rows = 0; rows < bitmap.height; rows++){
            bitmap.data[byteNumber] = buffer[byteNumber++];
            bitmap.data[byteNumber] = buffer[byteNumber++];
            bitmap.data[byteNumber] = buffer[byteNumber++];
        }
        if ((bitmap.width * 3) % 4 != 0){
            for (int i = 0; i < (bitmap.width * 3) % 4; i++){
                bitmap.data[byteNumber++] = 0;
                bitmap.data[byteNumber++] = 0;
            }
        }
    }
}

void BitmapLoader::readBytes(std::string filename, char * buffer, unsigned int n, unsigned int startpos) {
    std::ifstream file;
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try{
        file.open(filename, std::ios::binary);
        file.seekg(startpos);
        file.read(buffer, n);
    }
    catch (std::ifstream::failure e) {
        std::cerr << "Exception opening file: " << filename << " \n" << "What:\t" << e.what();
        std::exit(EXIT_FAILURE);
    }

    file.close();
}

template<typename T>
T BitmapLoader::hexConvert(std::string bytes){
    T accumulator = 0;
    for (int i = 0; i < bytes.length(); i++){
        accumulator += std::pow(16, 2*i) * (std::uint8_t)bytes[i];
    }
    return accumulator;
}

void DIBParser::parseBITMAPINFOHEADER(Bitmap &bitmap, std::string filename) {
    const unsigned int DIB_HEADER_LENGTH = 40;
    const unsigned int DIB_HEADER_OFFSET = 14;
    char buffer[DIB_HEADER_LENGTH];
    BitmapLoader::readBytes(filename, buffer, DIB_HEADER_LENGTH, DIB_HEADER_OFFSET);

    std::string s(&buffer[0], &buffer[DIB_HEADER_LENGTH]);
    bitmap.width = BitmapLoader::hexConvert<unsigned int>(s.substr(4, 4));
    bitmap.height = BitmapLoader::hexConvert<unsigned int>(s.substr(8, 4));
    bitmap.rawDataSize = BitmapLoader::hexConvert<unsigned int>(s.substr(20, 4));
    bitmap.data = new char[bitmap.rawDataSize];
}
#endif //UNTITLED_BITMAP_LOADER_H
