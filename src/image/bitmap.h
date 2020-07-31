//
// Created by DamianPC on 6/26/2020.
//

#ifndef UNTITLED_BITMAP_H
#define UNTITLED_BITMAP_H

#include <fstream>
#include <memory>

#include "../utils/shape.h"
#include "image.h"

namespace gen_algo_image {
    const unsigned int BMP_HEADER_LENGTH = 14;
    const unsigned int BITMAPINFOHEADER_LENGTH = 40;
    const std::string CLEAR_BMP_FILENAME = "../res/clear.bmp";

    class Bitmap : public Image {
        friend class BitmapLoader;

    public:
        unsigned int size;
        unsigned int pixelDataOffset;
        std::unique_ptr<char[]> data;

        Bitmap() {}

        Bitmap(const Bitmap &b2);

        Bitmap &operator=(const Bitmap &b2);

        void Create(unsigned int width, unsigned int height, Color color) override;

        unsigned int GetWidth() const override;

        unsigned int GetHeight() const override;

        void DrawPixel(int x, int y, Color color) override;

        Color GetPixelColor(int x, int y) override;

        void LoadToSFImage(sf::Image &img) override;

        void GetPixelColor(const int &x, const int &y, Color &color) override;

    private:
        unsigned int width;
        unsigned int paddedWidth;
        unsigned int height;

        void SetWidth(unsigned int width);

        void SetHeight(unsigned int height);
    };


/* <BitmapLoader> */

    class BitmapLoader : public ImageLoader {
    public:
        ~BitmapLoader() {
            if (buffer != nullptr) delete[]buffer;
        }

        void Load(Image &image, std::string filename) override;

        void Save(Image &image, std::string filename = "") override;

        static void ReadBytes(std::string filename, char *buffer, unsigned int n, unsigned int startpos = 0);

        template<typename T>
        static T HexConvert(std::string bytes);

    private:
        std::string filename;
        unsigned int dibHeaderLength;

        char *buffer;

        void ProcessHeader(Bitmap &bitmap);

        void ProcessDIBHeader(Bitmap &bitmap);

        void ProcessBody(Bitmap &bitmap);

        // DIB headers
        void ParseBITMAPINFOHEADER(Bitmap &bitmap);
    };
}

#endif //UNTITLED_BITMAP_H
