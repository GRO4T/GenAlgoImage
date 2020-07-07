//
// Created by DamianPC on 6/26/2020.
//

#ifndef UNTITLED_GENETIC_ALGORITHM_H
#define UNTITLED_GENETIC_ALGORITHM_H

#include <vector>
#include <map>
#include <set>
#include <SFML/Graphics/Image.hpp>
#include <type_traits>

#include "figures.h"
#include "image.h"
#include "sorted_container.h"

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


/* <Individual> */

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

/* <GeneticAlgorithm> */

template<class ImageType>
class GeneticAlgorithm{
    static_assert(std::is_base_of<Image, ImageType>::value, "ImageType must inherit from Image");
public:
    GeneticAlgorithm(unsigned int numSquares, unsigned int numCircles, ImageType &originalImage);

    void createPopulation(int size);
    void nextGeneration();
    void displayBestIndividual(unsigned int n = 0);
    void saveResultsToFiles(std::string filePrefix);

    Individual<ImageType>& getBestIndividual(){ return population.get(populationSize - 1); }
private:
    SortedContainer<Individual<ImageType>> population;

    unsigned int populationSize;
    unsigned int numSquares;
    unsigned int numCircles;
    ImageType& originalImage;
};

template<class ImageType>
void GeneticAlgorithm<ImageType>::createPopulation(int size) {
    this->populationSize = size;
    for (int i = 0; i < size; i++){
        auto individual = Individual<ImageType>(numSquares, numCircles, originalImage.getImageBounds());
        individual.randomize();
        individual.applyFiguresToResult();
        population.add(individual);
    }
}

template<class ImageType>
void GeneticAlgorithm<ImageType>::displayBestIndividual(unsigned int n) {
    if (n == 0) n = populationSize;
    population.sort();
    for (int i = 0; i < n; i++){
        std::cout << "score: " << population.get(i).getScore() << std::endl;
    }
}

template<class ImageType>
void GeneticAlgorithm<ImageType>::nextGeneration() {
    for (int i = 0; i < populationSize; i++){
        population.get(i).evaluate(originalImage);
    }
}

template<class ImageType>
GeneticAlgorithm<ImageType>::GeneticAlgorithm(unsigned int numSquares, unsigned int numCircles,
                                              ImageType &originalImage):numSquares(numSquares), numCircles(numCircles),
                                                                        originalImage(originalImage) {}

template<class ImageType>
void GeneticAlgorithm<ImageType>::saveResultsToFiles(std::string filePrefix) {
    for (int i = 0; i < populationSize; i++){
        population.get(i).saveResultToFile(filePrefix + std::to_string(i) + ".png");
    }
}

#endif //UNTITLED_GENETIC_ALGORITHM_H
