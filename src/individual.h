//
// Created by DamianPC on 7/19/2020.
//

#ifndef UNTITLED_INDIVIDUAL_H
#define UNTITLED_INDIVIDUAL_H

#include "image/image.h"
#include "figures.h"

namespace gen_algo_image {
    template<class ImageType>
    class Individual {
        static_assert(std::is_base_of<Image, ImageType>::value, "ImageType must inherit from Image");
    public:
        Individual(unsigned int numSquares, unsigned int numCircles, const Square imageBounds);

        Individual() {}

        Individual &operator=(Individual &i2) {
            this->squares = i2.squares;
            this->circles = i2.circles;
            this->result = i2.result;
            this->numSquares = i2.numSquares;
            this->numCircles = i2.numCircles;
            this->score = i2.score;
            this->imageBounds = i2.imageBounds;
            return *this;
        }

        float Evaluate(ImageType &original);

        void Mutate();

        void Crossover(Individual &mate);

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

    template<class ImageType>
    float Individual<ImageType>::Evaluate(ImageType &original) {
        float score = 0.0f;

        for (int y = 0; y < imageBounds.height; y += 2) {
            for (int x = 0; x < imageBounds.width; x += 2) {
                Color originalColor = original.GetPixelColor(x, y);
                Color color = result.GetPixelColor(x, y);
                Color diff = color.Diff(originalColor);
                float pixelScore = 1.0f - (diff.red + diff.green + diff.blue) / 765.0f;
                score += 4.0f * pixelScore;
            }
        }

        score /= (float) (imageBounds.width * imageBounds.height);
        this->score = score;
        return score;
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

#endif //UNTITLED_INDIVIDUAL_H
