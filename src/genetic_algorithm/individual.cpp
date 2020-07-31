//
// Created by damian on 7/31/20.
//

#include "individual.h"

namespace gen_algo_image{
    template<class ImageType>
    Individual<ImageType>& Individual<ImageType>::operator=(const Individual &i2) {
        this->squares = i2.squares;
        this->circles = i2.circles;
        this->result = i2.result;
        this->numSquares = i2.numSquares;
        this->numCircles = i2.numCircles;
        this->score = i2.score;
        this->imageBounds = i2.imageBounds;
        return *this;
    }

    template<class ImageType>
    float Individual<ImageType>::Evaluate(ImageType &original) {
        float score = 0.0f, pixelScore;
        Color originalColor, color;
        for (int y = 0; y < imageBounds.height; ++y) {
            for (int x = 0; x < imageBounds.width; ++x) {
                original.GetPixelColor(x, y, originalColor);
                result.GetPixelColor(x, y, color);
                //Color diff = color.Diff(originalColor);
                //pixelScore = 1.0f - (diff.red + diff.green + diff.blue) / 765.0f;
                //score += pixelScore;
                if (color == originalColor) score++;
            }
        }

        //score /= (float) (imageBounds.width * imageBounds.height);
        this->score = score;
        return score;
    }

    template<class ImageType>
    Individual<ImageType> Individual<ImageType>::Crossover(Individual &mate){
        std::cout << "crossover" << std::endl;
        Individual<ImageType> child(this->numSquares, this->numCircles, this->imageBounds);
        for (int i = 0; i < numSquares / 2; i++){
            child.squares.push_back(this->squares.at(i));
        }
        for (int i = numSquares / 2; i < numSquares; i++){
            child.squares.push_back(mate.squares.at(i));
        }
/*
            child.ApplyFiguresToResult();
            child.SaveResultToFile("../res/child.bmp");
            this->SaveResultToFile("../res/parent1.bmp");
            mate.SaveResultToFile("../res/parent2.bmp");
            */

        return child;
    }

    template<class ImageType>
    void Individual<ImageType>::Randomize() {
        for (int i = 0; i < numSquares; i++) {
            squares.push_back(ColoredSquare(imageBounds));
        }
        for (int i = 0; i < numCircles; i++) {
            circles.push_back(ColoredCircle(imageBounds));
        }
    }

    template<class ImageType>
    void Individual<ImageType>::LoadResultToSFImage(sf::Image &img) {
        result.LoadToSFImage(img);
    }

    template<class ImageType>
    void Individual<ImageType>::SaveResultToFile(std::string filename) {
        sf::Image img;
        result.LoadToSFImage(img);
        img.saveToFile(filename);
    }

    template<class ImageType>
    Individual<ImageType>::Individual(unsigned int numSquares, unsigned int numCircles, const Square imageBounds)
            :numSquares(numSquares), numCircles(numCircles), imageBounds(imageBounds) {
        Individual::result.Create(
                imageBounds.width - imageBounds.x,
                imageBounds.height - imageBounds.y,
                Color(255, 255, 255)
        );
    }

    template<class ImageType>
    void Individual<ImageType>::ApplyFiguresToResult() {
        for (ColoredSquare coloredSquare : squares) {
            result.DrawSquare(coloredSquare.square, coloredSquare.color);
        }
        for (ColoredCircle coloredCircle : circles) {
            result.DrawCircle(coloredCircle.circle, coloredCircle.color);
        }
    }

    template<class ImageType>
    float Individual<ImageType>::GetScore() const {
        return score;
    }
}