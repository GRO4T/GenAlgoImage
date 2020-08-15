//
// Created by damian on 7/31/20.
//

#include "genetic_algorithm.h"

namespace gen_algo_image {
    template<class ImageType>
    void GeneticAlgorithm<ImageType>::CreatePopulation() {
        for (int i = 0; i < populationSize; i++) {
            Individual<ImageType> individual(numSquares, numCircles, originalImage->GetImageBounds());
            individual.Randomize();
            individual.ApplyFiguresToResult();
            population.Add(individual);
        }
    }

    template<class ImageType>
    void GeneticAlgorithm<ImageType>::NextGeneration() {
        Timer timer;

        timer.Start();

        FitnessFunctionCalculation();
        Crossover();
        Mutation();
        Selection();

        timer.Stop();

        std::cout << "gen time : ";
        timer.Time();
    }

    template<class ImageType>
    void GeneticAlgorithm<ImageType>::FitnessFunctionCalculation() {
        for (int i = 0; i < populationSize; i++) {
            Individual<ImageType> *individual = &population.Get(i);
            individual->Evaluate(*originalImage);
        }
        population.Sort();

        SetBestIndividual(population.Get(0));
        std::cout << "Best " << GetBestIndividual() << std::endl;

        // display scores
        for (int i = 0; i < populationSize; i++) {
            std::cout << population.Get(i) << std::endl;
        }
    }

    template<class ImageType>
    void GeneticAlgorithm<ImageType>::Crossover() {
        /*
        for (int i = 0; i < offspringSize; i++){

        }
         */
        float sum = 0.01f;
        float probabilityArray[populationSize];
        for (int i = 0; i < populationSize; i++) {
            sum += population.Get(i).GetScore();
        }
        for (int i = 0; i < populationSize; i++) {
            probabilityArray[i] = population.Get(i).GetScore() / sum;
            std::cout << i << " prob : " << probabilityArray[i] << std::endl;
        }

        for (int i = 0; i < 2; i++) {
            Individual<ImageType> *mother = sum > 0.5f ? nullptr : &population.Get(0);
            Individual<ImageType> *father = sum > 0.5f ? nullptr : &population.Get(1);

            int j = 0;
            while (mother == nullptr) {
                float number = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
                if (number <= probabilityArray[j]) {
                    std::cout << "found mother (number = " << number << ") (j=" << j << ") ";
                    mother = &population.Get(j);
                    std::cout << *mother << std::endl;
                }
                j = (j + 1) % populationSize;
            }
            while (father == nullptr) {
                float number = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
                if (number <= probabilityArray[j]) {
                    std::cout << "found father (number = " << number << ") (j=" << j << ") ";
                    father = &population.Get(j);
                    std::cout << *father << std::endl;
                }
                j = (j + 1) % populationSize;
            }
            population.Add(mother->Crossover(*father));
        }
    }

    template<class ImageType>
    void GeneticAlgorithm<ImageType>::Mutation() {

    }

    template<class ImageType>
    void GeneticAlgorithm<ImageType>::Selection() {
        Container<Individual<ImageType>> temp;
        temp.Reserve(populationSize);

        // create next gen
        for (int i = 0; i < 10; i++) {
            temp.Add(population.Get(i));
        }
        for (int i = 8; i < populationSize; i++) {
            Individual<ImageType> individual(numSquares, numCircles, originalImage->GetImageBounds());
            individual.Randomize();
            individual.ApplyFiguresToResult();
            temp.Add(individual);
        }

        population = temp;
    }

/*
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
    */

    template<class ImageType>
    GeneticAlgorithm<ImageType>::GeneticAlgorithm(unsigned int numSquares, unsigned int numCircles,
                                                  ImageType *originalImage)
            : numSquares(numSquares), numCircles(numCircles),
              originalImage(originalImage) {
    }
}