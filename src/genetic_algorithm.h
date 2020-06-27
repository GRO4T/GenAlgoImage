//
// Created by DamianPC on 6/26/2020.
//

#ifndef UNTITLED_GENETIC_ALGORITHM_H
#define UNTITLED_GENETIC_ALGORITHM_H

#include <vector>
#include <SFML/Graphics/Image.hpp>
#include <type_traits>

#include "figures.h"
#include "image.h"

#define MIN_CIRCLE_RADIUS 10.0f

/* Classes in this file (search using <classname> pattern, e.g. <Bitmap>)
 *      ColoredSquare
 *      ColoredCircle
 *      GeneticAlgorithm
 *      Individual
 */

/* <ColoredSquare> */

class ColoredSquare{
public:
    ColoredSquare(const Square &square, const Color &color) : square(square), color(color) {}
    ColoredSquare(Square bounds) { square.randomize(bounds); color = Color(); }
    Square square;
    Color color;
};

/* <ColoredCircle> */

class ColoredCircle{
public:
    ColoredCircle(const Circle &circle, const Color &color) : circle(circle), color(color) {}
    ColoredCircle(Square bounds) { circle.randomize(bounds); color = Color(); }
    Circle circle;
    Color color;
};

/* <GeneticAlgorithm> */

class GeneticAlgorithm{
};

/* <Individual> */

template<class ImageType>
class Individual{
    static_assert(std::is_base_of<Image, ImageType>::value, "ImageType must inherit from Image");
public:
    Individual(unsigned int numSquares, unsigned int numCircles, const Square &imageBounds);
    Individual() {}

    float evaluate(ImageType original);
    void mutate();
    void crossover(Individual& mate);
    void randomize();

    void loadResultToSFImage(sf::Image& img);

    void setNumSquares(unsigned int numSquares);
    void setNumCircles(unsigned int numCircles);
    void setImageBounds(const Square &imageBounds);

private:
    std::vector<ColoredSquare> squares;
    std::vector<ColoredCircle> circles;

    ImageType result;

    unsigned int numSquares;
    unsigned int numCircles;

    Square imageBounds;
};

template<class ImageType>
float Individual<ImageType>::evaluate(ImageType original) {
    float score = 0.0f;
    for (int y = 0; y < imageBounds.height; y++){
        for (int x = 0; x < imageBounds.width; x++){
            Color originalColor = original.getPixelColor(x, y);
            Color color = result.getPixelColor(x, y);
            Color diff = color.diff(originalColor);
            float pixelScore = 1.0f - (diff.red + diff.green + diff.blue) / 765.0f;
            score += pixelScore;
        }
    }

    score /= (float)(imageBounds.width * imageBounds.height);
    return score;
}

template<class ImageType>
void Individual<ImageType>::randomize() {
    for (int i = 0; i < numSquares; i++){
        squares.push_back(ColoredSquare(imageBounds));
    }
    for (int i = 0; i < numCircles; i++){
        circles.push_back(ColoredCircle(imageBounds));
    }
}

template<class ImageType>
void Individual<ImageType>::loadResultToSFImage(sf::Image &img) {
    for (ColoredSquare coloredSquare : squares){
        result.drawSquare(coloredSquare.square, coloredSquare.color);
    }
    for (ColoredCircle coloredCircle : circles){
        result.drawCircle(coloredCircle.circle, coloredCircle.color);
    }

    result.loadToSFImage(img);
}

template<class ImageType>
Individual<ImageType>::Individual(unsigned int numSquares, unsigned int numCircles, const Square &imageBounds)
        :numSquares(numSquares), numCircles(numCircles), imageBounds(imageBounds) {
    Individual::result.create(
            imageBounds.width - imageBounds.x,
            imageBounds.height - imageBounds.y,
            Color(255, 255, 255)
    );
}

template<class ImageType>
void Individual<ImageType>::setNumSquares(unsigned int numSquares) {
    Individual::numSquares = numSquares;
}

template<class ImageType>
void Individual<ImageType>::setNumCircles(unsigned int numCircles) {
    Individual::numCircles = numCircles;
}

template<class ImageType>
void Individual<ImageType>::setImageBounds(const Square &imageBounds) {
    Individual::imageBounds = imageBounds;
    Individual::result.create(
            imageBounds.width - imageBounds.x,
            imageBounds.height - imageBounds.y,
            Color()
    );
}


#endif //UNTITLED_GENETIC_ALGORITHM_H
