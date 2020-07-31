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

#include "../utils/timer.h"
#include "individual.h"
#include "../utils/container.h"

namespace gen_algo_image {
    template<class ImageType>
    class GeneticAlgorithm {
        static_assert(std::is_base_of<Image, ImageType>::value, "ImageType must inherit from Image");
    public:
        GeneticAlgorithm(unsigned int numSquares, unsigned int numCircles, ImageType *originalImage);

        void CreatePopulation();

        void NextGeneration();

        void FitnessFunctionCalculation();

        void Crossover();

        void Mutation();

        Individual<ImageType> GetBestIndividual() {
            return bestIndividual;
        }

        //static unsigned genNumber = 0;
    private:
        Container<Individual<ImageType>> population;
        //std::vector<Individual<ImageType>> population;

        const unsigned int populationSize = 20;
        unsigned int numSquares;
        unsigned int numCircles;
        const unsigned offspringSize = 50;
        ImageType *originalImage;

        Individual<ImageType> bestIndividual;
    };

    // templates instantiation
    template class GeneticAlgorithm<Bitmap>;
    template class GeneticAlgorithm<SFML_ImageWrapper>;
    template class GeneticAlgorithm<ArashPartowBitmapWrapper>;
}


#endif //UNTITLED_GENETIC_ALGORITHM_H
