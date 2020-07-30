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

#include "timeit.h"
#include "individual.h"
#include "sorted_container.h"

namespace gen_algo_image{
    template<class ImageType>
    class GeneticAlgorithm{
        static_assert(std::is_base_of<Image, ImageType>::value, "ImageType must inherit from Image");
    public:
        GeneticAlgorithm(unsigned int numSquares, unsigned int numCircles, ImageType* originalImage);
        void CreatePopulation(int size);
        void NextGeneration();

        Individual<ImageType> GetBestIndividual(){
            return bestIndividual;
        }
    private:
        SortedContainer<Individual<ImageType>> population;
        //std::vector<Individual<ImageType>> population;

        unsigned int populationSize;
        unsigned int numSquares;
        unsigned int numCircles;
        ImageType* originalImage;

        Individual<ImageType> bestIndividual;
    };

    template<class ImageType>
    void GeneticAlgorithm<ImageType>::CreatePopulation(int size) {
        this->populationSize = size;
        for (int i = 0; i < size; i++){
            Individual<ImageType> individual(numSquares, numCircles, originalImage->GetImageBounds());
            individual.Randomize();
            individual.ApplyFiguresToResult();
            population.Add(individual);
        }
    }

    template<class ImageType>
    void GeneticAlgorithm<ImageType>::NextGeneration() {
        Timeit stoper;

        for (int i = 0; i < populationSize; i++){
            Individual<ImageType>* individual = &population.Get(i);
            stoper.Start();
            individual->Clear();
            stoper.Stop();

            std::cout << "clear ";
            stoper.Time();

            stoper.Start();
            individual->Randomize();
            stoper.Stop();

            std::cout << "randomize ";
            stoper.Time();

            stoper.Start();
            individual->ApplyFiguresToResult();
            stoper.Stop();

            std::cout << "apply ";
            stoper.Time();

            stoper.Start();
            individual->Evaluate(*originalImage);
            stoper.Stop();
            std::cout << "evaluate ";
            stoper.Time();
        }
        population.Sort();
        bestIndividual = population.Get(0);

        // display scores
        for (int i = 0; i < 5; i++){
            std::cout << "score : " << population.Get(i).GetScore() << std::endl;
        }
    }

    template<class ImageType>
    GeneticAlgorithm<ImageType>::GeneticAlgorithm(unsigned int numSquares, unsigned int numCircles,
                                                  ImageType* originalImage)
                                                  : numSquares(numSquares), numCircles(numCircles),
                                                  originalImage(originalImage) {}
}

#endif //UNTITLED_GENETIC_ALGORITHM_H
