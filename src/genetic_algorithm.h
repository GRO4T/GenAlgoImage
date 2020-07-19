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

template<class ImageType>
class GeneticAlgorithm{
    static_assert(std::is_base_of<Image, ImageType>::value, "ImageType must inherit from Image");
public:
    GeneticAlgorithm(unsigned int numSquares, unsigned int numCircles, ImageType* originalImage);
    void createPopulation(int size);
    void nextGeneration();
    void displayBestIndividual(unsigned int n = 0);
    void saveResultsToFiles(std::string filePrefix);

    Individual<ImageType>* getBestIndividual(){
        //population.sort();
        return &population.get(populationSize - 1);
    }
private:
    SortedContainer<Individual<ImageType>> population;
    //std::vector<Individual<ImageType>> population;

    unsigned int populationSize;
    unsigned int numSquares;
    unsigned int numCircles;
    ImageType* originalImage;
};

template<class ImageType>
void GeneticAlgorithm<ImageType>::createPopulation(int size) {
    Timeit stoper;

    this->populationSize = size;
    for (int i = 0; i < size; i++){
        Individual<ImageType> individual(numSquares, numCircles, originalImage->getImageBounds());
        individual.randomize();

        stoper.start();
        individual.applyFiguresToResult();
        stoper.stop();
        //population.push_back(individual);
        population.add(individual);

        std::cout << "apply figures " << i << " ";
        stoper.time();
    }
}

template<class ImageType>
void GeneticAlgorithm<ImageType>::displayBestIndividual(unsigned int n) {
    if (n == 0) n = populationSize;
    int lastId = populationSize - n - 1;
    //population.sort();
    for (int i = populationSize - 1; i > lastId; i--){
        std::cout << "score: " << population.get(i).getScore() << std::endl;
    }
}

template<class ImageType>
void GeneticAlgorithm<ImageType>::nextGeneration() {
    Timeit stoper;

    //population.sort();
    for (int i = populationSize - 1; i >= 0; i--){
        Individual<ImageType>* individual = &population.get(i);
        stoper.start();
        individual->clear();
        stoper.stop();

        std::cout << "clear ";
        stoper.time();

        stoper.start();
        individual->randomize();
        stoper.stop();

        std::cout << "randomize ";
        stoper.time();

        stoper.start();
        individual->applyFiguresToResult();
        stoper.stop();

        std::cout << "apply ";
        stoper.time();
    }

    stoper.start();
    for (int i = 0; i < populationSize; i++){
        Individual<ImageType>* ind = &population.get(i);
        ind->evaluate(*originalImage);
    }
    stoper.stop();
    std::cout << "evaluate ";
    stoper.time();
}

template<class ImageType>
GeneticAlgorithm<ImageType>::GeneticAlgorithm(unsigned int numSquares, unsigned int numCircles,
                                              ImageType* originalImage):numSquares(numSquares), numCircles(numCircles),
                                                                        originalImage(originalImage) {}

template<class ImageType>
void GeneticAlgorithm<ImageType>::saveResultsToFiles(std::string filePrefix) {
    for (int i = 0; i < populationSize; i++){
        population.get(i).saveResultToFile(filePrefix + std::to_string(i) + ".png");
    }
}

#endif //UNTITLED_GENETIC_ALGORITHM_H
