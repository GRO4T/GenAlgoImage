//
// Created by damian on 7/31/20.
//

#include "bitmap.h"

namespace gen_algo_image {
    Bitmap::Bitmap(const Bitmap &b2) {
        this->size = b2.size;
        this->pixelDataOffset = b2.pixelDataOffset;

        this->data = std::make_unique<char[]>(this->size);
        memcpy(data.get(), b2.data.get(), this->size);
        SetWidth(b2.GetWidth());
        SetHeight(b2.GetHeight());
    }

    Bitmap &Bitmap::operator=(const Bitmap &b2) {
        this->size = b2.size;
        this->pixelDataOffset = b2.pixelDataOffset;

        this->data = std::make_unique<char[]>(this->size);
        memcpy(data.get(), b2.data.get(), this->size);
        SetWidth(b2.GetWidth());
        SetHeight(b2.GetHeight());
        return *this;
    }

    void Bitmap::Create(unsigned int width, unsigned int height, Color color) {
        unsigned char bmpfileheader[14] = {'B', 'M', 0, 0, 0, 0, 0, 0, 0, 0, 54, 0, 0, 0};
        unsigned char bmpinfoheader[40] = {40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 24, 0};

        SetHeight(height);
        SetWidth(width);
        size = 54 + paddedWidth * height;
        pixelDataOffset = 54;
        data = std::make_unique<char[]>(size);

        int filesize = size;
        int w = width;
        int h = height;

        bmpfileheader[2] = (unsigned char) (filesize);
        bmpfileheader[3] = (unsigned char) (filesize >> 8);
        bmpfileheader[4] = (unsigned char) (filesize >> 16);
        bmpfileheader[5] = (unsigned char) (filesize >> 24);

        bmpinfoheader[4] = (unsigned char) (w);
        bmpinfoheader[5] = (unsigned char) (w >> 8);
        bmpinfoheader[6] = (unsigned char) (w >> 16);
        bmpinfoheader[7] = (unsigned char) (w >> 24);
        bmpinfoheader[8] = (unsigned char) (h);
        bmpinfoheader[9] = (unsigned char) (h >> 8);
        bmpinfoheader[10] = (unsigned char) (h >> 16);
        bmpinfoheader[11] = (unsigned char) (h >> 24);

        memcpy(data.get(), bmpfileheader, 14);
        memcpy(data.get() + 14, bmpinfoheader, 40);

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                DrawPixel(x, y, color);
            }
        }
    }

    unsigned int Bitmap::GetWidth() const {
        return width;
    }

    unsigned int Bitmap::GetHeight() const {
        return height;
    }

    void Bitmap::DrawPixel(int x, int y, Color color) {
        //if (x >= width | x < 0 | y >= height | y < 0 | width < 0 | height < 0)
        //    return;
        int byteNumber = y * paddedWidth + 3 * x + pixelDataOffset;
        data.get()[byteNumber] = color.red;
        data.get()[byteNumber + 1] = color.green;
        data.get()[byteNumber + 2] = color.blue;
    }

    Color Bitmap::GetPixelColor(int x, int y) {
        int byteNumber = y * paddedWidth + 3 * x + pixelDataOffset;
        return Color(data[byteNumber], data[byteNumber + 1], data[byteNumber + 2]);
    }

    void Bitmap::GetPixelColor(const int &x, const int &y, Color &color) {
        const int byteNumber = y * paddedWidth + 3 * x + pixelDataOffset;
        color.red = data[byteNumber];
        color.green = data[byteNumber + 1];
        color.blue = data[byteNumber + 2];
    }


    void Bitmap::SetWidth(unsigned int width) {
        Bitmap::width = width;
        paddedWidth = 3 * width + ((4 - width % 4) % 4);
    }

    void Bitmap::SetHeight(unsigned int height) {
        Bitmap::height = height;
    }

    void Bitmap::LoadToSFImage(sf::Image &img) {
        img.loadFromMemory(data.get(), size);
    }


    void BitmapLoader::Load(Image &image, std::string filename) {
        this->filename = filename;
        Bitmap &bitmap = (Bitmap &) image;
        try {
            ProcessHeader(bitmap);
            ProcessDIBHeader(bitmap);
            ProcessBody(bitmap);
        }
        catch (std::runtime_error e) {
            std::cerr << e.what() << std::endl;
            return;
        }
    }

    void BitmapLoader::Save(Image &image, std::string filename) {
        if (filename == "") filename = this->filename;

        Bitmap &bitmap = (Bitmap &) image;
        std::ofstream file;
        file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try {
            file.open(filename, std::ios::out | std::ios::binary);
            file.write(bitmap.data.get(), bitmap.size);
        }
        catch (std::ofstream::failure e) {
            std::cerr << "Exception opening file: " << filename << " \n" << "What:\t" << e.what();
            std::exit(EXIT_FAILURE);
        }
    }

    void BitmapLoader::ProcessHeader(Bitmap &bitmap) {
        char header[BMP_HEADER_LENGTH];
        ReadBytes(this->filename, header, BMP_HEADER_LENGTH);
        std::string stringBuffer(&header[0], &header[BMP_HEADER_LENGTH]);
        bitmap.size = HexConvert<unsigned int>(stringBuffer.substr(2, 4));

        this->buffer = new char[bitmap.size];
    }

    void BitmapLoader::ProcessDIBHeader(Bitmap &bitmap) {
        ReadBytes(this->filename, this->buffer, bitmap.size, 0);
        std::string buf(&(this->buffer[BMP_HEADER_LENGTH]), &(this->buffer[BMP_HEADER_LENGTH + 4]));
        dibHeaderLength = HexConvert<unsigned int>(buf);
        switch (dibHeaderLength) {
            case 40:
                ParseBITMAPINFOHEADER(bitmap);
                break;
            default:
                throw std::runtime_error("DIB Header not supported!");
        }
    }

    void BitmapLoader::ProcessBody(Bitmap &bitmap) {
        if (!bitmap.data) {
            bitmap.data = std::make_unique<char[]>(bitmap.size);
        }
        memcpy(bitmap.data.get(), this->buffer, bitmap.size);
    }

    void BitmapLoader::ReadBytes(std::string filename, char *buffer, unsigned int n, unsigned int startpos) {
        std::ifstream file;
        file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try {
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
    T BitmapLoader::HexConvert(std::string bytes) {
        T accumulator = 0;
        for (int i = 0; i < bytes.length(); i++) {
            accumulator += std::pow(16, 2 * i) * (std::uint8_t) bytes[i];
        }
        return accumulator;
    }

    void BitmapLoader::ParseBITMAPINFOHEADER(Bitmap &bitmap) {
        bitmap.pixelDataOffset = 54;
        std::string stringBuffer1(&(this->buffer[BMP_HEADER_LENGTH + 4]), &(this->buffer[BMP_HEADER_LENGTH + 8]));
        bitmap.SetWidth(HexConvert<unsigned int>(stringBuffer1));
        std::string stringBuffer2(&(this->buffer[BMP_HEADER_LENGTH + 8]), &(this->buffer[BMP_HEADER_LENGTH + 12]));
        bitmap.SetHeight(HexConvert<unsigned int>(stringBuffer2));
    }
}
