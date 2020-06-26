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

const unsigned int BMP_HEADER_LENGTH = 14;

class Bitmap{
public:
    unsigned int size;
    unsigned int width;
    unsigned int height;
    unsigned int pixelDataOffset;
    char * data;
    ~Bitmap(){
        if (data != nullptr){
            delete data;
        }
    }
};

class BitmapLoader{
public:
    ~BitmapLoader(){
        if (buffer != nullptr) delete buffer;
    }

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

    char * buffer;
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
    std::cout << dibHeaderLength << std::endl;
    switch(dibHeaderLength){
        case 40: bitmap.pixelDataOffset = BMP_HEADER_LENGTH + dibHeaderLength; break;
        default: throw std::runtime_error("DIB Header not supported!");
    }
}

void BitmapLoader::processBody(Bitmap &bitmap) {
    bitmap.data = new char[bitmap.size];
    memcpy(bitmap.data, this->buffer, bitmap.size);

    /*
    const unsigned int PIXEL_DATA_SIZE = bitmap.size - bitmap.pixelDataOffset;
    char buffer[PIXEL_DATA_SIZE];
    readBytes(this->filename, buffer, PIXEL_DATA_SIZE, bitmap.pixelDataOffset);

    unsigned int byteNumber = 0;
    for (unsigned int cols = 0; cols < bitmap.width; cols++){
        for (unsigned int rows = 0; rows < bitmap.height; rows++){
            bitmap.data[byteNumber + bitmap.pixelDataOffset] = buffer[byteNumber++];
            bitmap.data[byteNumber + bitmap.pixelDataOffset] = buffer[byteNumber++];
            bitmap.data[byteNumber + bitmap.pixelDataOffset] = buffer[byteNumber++];
        }
        if ((bitmap.width * 3) % 4 != 0){
            for (int i = 0; i < (bitmap.width * 3) % 4; i++){
                bitmap.data[(byteNumber++) + bitmap.pixelDataOffset] = 0;
                bitmap.data[(byteNumber++) + bitmap.pixelDataOffset] = 0;
            }
        }
    }*/


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

#endif //UNTITLED_BITMAP_LOADER_H
