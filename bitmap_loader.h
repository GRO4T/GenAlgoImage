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
#include <assert.h>
#include <cstring>

#include "bitmap.h"

class BitmapLoader{
public:
    ~BitmapLoader(){
        if (buffer != nullptr) delete buffer;
    }

    void load(Bitmap & bitmap, std::string filename);
    void save(Bitmap & bitmap, std::string filename = "");

    static void readBytes(std::string filename, char * buffer, unsigned int n, unsigned int startpos = 0);
    template<typename T>
    static T hexConvert(std::string bytes);
private:
    std::string filename;
    unsigned int dibHeaderLength;

    char * buffer;

    void processHeader(Bitmap & bitmap);
    void processDIBHeader(Bitmap & bitmap);
    void processBody(Bitmap & bitmap);

    // DIB headers
    void parseBITMAPINFOHEADER(Bitmap & bitmap);
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

void BitmapLoader::save(Bitmap &bitmap, std::string filename) {
    if (filename == "") filename = this->filename;

    std::ofstream file;
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try{
        file.open(filename, std::ios::out | std::ios::binary);
        file.write(bitmap.data, bitmap.size);
    }
    catch (std::ofstream::failure e) {
        std::cerr << "Exception opening file: " << filename << " \n" << "What:\t" << e.what();
        std::exit(EXIT_FAILURE);
    }
}

void BitmapLoader::processHeader(Bitmap & bitmap) {
    char header[BMP_HEADER_LENGTH];
    readBytes(this->filename, header, BMP_HEADER_LENGTH);
    std::string stringBuffer(&header[0], &header[BMP_HEADER_LENGTH]);
    bitmap.size = hexConvert<unsigned int>(stringBuffer.substr(2, 4));

    this->buffer = new char[bitmap.size];
}

void BitmapLoader::processDIBHeader(Bitmap & bitmap) {
    readBytes(this->filename, this->buffer, bitmap.size, 0);
    std::string buf(&(this->buffer[BMP_HEADER_LENGTH]), &(this->buffer[BMP_HEADER_LENGTH + 4]));
    dibHeaderLength = hexConvert<unsigned int>(buf);
    switch(dibHeaderLength){
        case 40: parseBITMAPINFOHEADER(bitmap); break;
        default: throw std::runtime_error("DIB Header not supported!");
    }
}

void BitmapLoader::processBody(Bitmap &bitmap) {
    bitmap.data = new char[bitmap.size];
    memcpy(bitmap.data, this->buffer, bitmap.size);
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
}

template<typename T>
T BitmapLoader::hexConvert(std::string bytes){
    T accumulator = 0;
    for (int i = 0; i < bytes.length(); i++){
        accumulator += std::pow(16, 2*i) * (std::uint8_t)bytes[i];
    }
    return accumulator;
}

void BitmapLoader::parseBITMAPINFOHEADER(Bitmap &bitmap) {
    bitmap.pixelDataOffset = 54;
    std::string stringBuffer1(&(this->buffer[BMP_HEADER_LENGTH + 4]),&(this->buffer[BMP_HEADER_LENGTH + 8]));
    bitmap.setWidth(hexConvert<unsigned int>(stringBuffer1));
    std::string stringBuffer2(&(this->buffer[BMP_HEADER_LENGTH + 8]),&(this->buffer[BMP_HEADER_LENGTH + 12]));
    bitmap.setHeight(hexConvert<unsigned int>(stringBuffer2));
}

#endif //UNTITLED_BITMAP_LOADER_H
