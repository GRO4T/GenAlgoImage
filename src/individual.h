//
// Created by DamianPC on 7/19/2020.
//

#ifndef UNTITLED_INDIVIDUAL_H
#define UNTITLED_INDIVIDUAL_H

#include "image/image.h"
#include "figures.h"

template<class ImageType>
class Individual{
    static_assert(std::is_base_of<Image, ImageType>::value, "ImageType must inherit from Image");
public:
    Individual(unsigned int numSquares, unsigned int numCircles, const Square imageBounds);
    Individual() {}

    float evaluate(ImageType& original);
    void mutate();
    void crossover(Individual& mate);
    void randomize();

    void loadResultToSFImage(sf::Image& img);
    void saveResultToFile(std::string filename);
    void applyFiguresToResult();
    void clear(){ squares.clear(); circles.clear(); result.clearColor(Color(255, 255, 255)); }

    void setNumSquares(unsigned int numSquares);
    void setNumCircles(unsigned int numCircles);
    void setImageBounds(const Square &imageBounds);

    float getScore() const;

    friend bool operator<(const Individual<ImageType>& l, const Individual<ImageType>& r){
        return l.getScore() < r.getScore();
    }

private:
    std::vector<ColoredSquare> squares;
    std::vector<ColoredCircle> circles;

    ImageType result;

    unsigned int numSquares;
    unsigned int numCircles;

    Square imageBounds;

    float score = 0.0f;
};

template<class ImageType>
float Individual<ImageType>::evaluate(ImageType& original) {
    float score = 0.0f;

    for (int y = 0; y < imageBounds.height; y+= 1){
        for (int x = 0; x < imageBounds.width; x+= 1){
            Color originalColor = original.getPixelColor(x, y);
            Color color = result.getPixelColor(x, y);
            Color diff = color.diff(originalColor);
            float pixelScore = 1.0f - (diff.red + diff.green + diff.blue) / 765.0f;
            score += pixelScore;
        }
    }

    score /= (float)(imageBounds.width * imageBounds.height);
    this->score = score;
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
    result.loadToSFImage(img);
}

template<class ImageType>
void Individual<ImageType>::saveResultToFile(std::string filename) {
    sf::Image img;
    result.loadToSFImage(img);
    img.saveToFile(filename);
}

template<class ImageType>
Individual<ImageType>::Individual(unsigned int numSquares, unsigned int numCircles, const Square imageBounds)
        :numSquares(numSquares), numCircles(numCircles), imageBounds(imageBounds) {
    Individual::result.create(
            imageBounds.width - imageBounds.x,
            imageBounds.height - imageBounds.y,
            Color(255, 255, 255)
    );
}

template<class ImageType>
void Individual<ImageType>::applyFiguresToResult() {
    for (ColoredSquare coloredSquare : squares){
        result.drawSquare(coloredSquare.square, coloredSquare.color);
    }
    for (ColoredCircle coloredCircle : circles){
        result.drawCircle(coloredCircle.circle, coloredCircle.color);
    }
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

template<class ImageType>
float Individual<ImageType>::getScore() const {
    return score;
}

#endif //UNTITLED_INDIVIDUAL_H
