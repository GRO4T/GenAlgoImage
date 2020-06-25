#include <iostream>
#include <fstream>
#include <sstream>

int main() {
    std::cout << "Hello, World!" << std::endl;
    char buffer[14];

    std::string filename = "../test.bmp";
    std::ifstream bitmapFile;
    bitmapFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try{
        bitmapFile.open(filename, std::ios::binary);
        bitmapFile.read(buffer, 14);
    }
    catch (std::ifstream::failure e) {
        std::cerr << "Exception opening file: " << filename << " \n" << "What:\t" << e.what();
        std::exit(EXIT_FAILURE);
    }

    for (int i = 0; i < 14; ++i){
        printf("%02hhx", buffer[i]);
    }

    //convert hex to int
    unsigned int size;
    std::string sizeAsHex;
    std::stringstream ss;
    for (int i = 5; i > 1; i--){
        sizeAsHex += buffer[i];
    }
    ss << std::hex << sizeAsHex;
    std::cout << sizeAsHex;
    size = lexical_cast<int>(sizeAsHex);

    std::cout << "size: " << size << std::endl;

    std::cout << std::endl;

    return 0;
}

void convert
