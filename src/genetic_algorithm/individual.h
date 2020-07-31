//
// Created by DamianPC on 7/19/2020.
//

#ifndef UNTITLED_INDIVIDUAL_H
#define UNTITLED_INDIVIDUAL_H

#include "../image/image.h"
#include "../image/bitmap.h"
#include "../image/sfml_image_wrapper.h"
#include "../image/arash_partow_bitmap_wrapper.h"

namespace gen_algo_image {
    template<class ImageType>
    class Individual {
        static_assert(std::is_base_of<Image, ImageType>::value, "ImageType must inherit from Image");
    public:
        Individual(unsigned int numSquares, unsigned int numCircles, const Square imageBounds);

        Individual() {}

        Individual &operator=(const Individual &i2);

        float Evaluate(ImageType &original);

        void Mutate();

        Individual Crossover(Individual &mate);

        void Randomize();

        void LoadResultToSFImage(sf::Image &img);

        void SaveResultToFile(std::string filename);

        void ApplyFiguresToResult();

        void Clear() {
            squares.clear();
            circles.clear();
            result.ClearColor(clearColor);
        }


        float GetScore() const;

        friend bool operator<(const Individual<ImageType> &l, const Individual<ImageType> &r) {
            return l.GetScore() > r.GetScore();
        }

    private:
        const Color clearColor = Color(255, 255, 255);

        std::vector<ColoredSquare> squares;
        std::vector<ColoredCircle> circles;

        ImageType result;

        unsigned int numSquares;
        unsigned int numCircles;

        Square imageBounds;

        float score = 0.0f;
    };

    // templates instantiation
    template class Individual<Bitmap>;
    template class Individual<SFML_ImageWrapper>;
    template class Individual<ArashPartowBitmapWrapper>;
}

#endif //UNTITLED_INDIVIDUAL_H
