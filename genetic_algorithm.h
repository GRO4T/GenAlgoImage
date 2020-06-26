//
// Created by DamianPC on 6/26/2020.
//

#ifndef UNTITLED_GENETIC_ALGORITHM_H
#define UNTITLED_GENETIC_ALGORITHM_H


class GeneticAlgorithm{
};

class Individual{
public:
    virtual float evaluate() = 0;
    virtual void mutate() = 0;
    virtual void crossover(Individual &mate) = 0;

};

class SquareIndividual : Individual{
public:
    float
};
#endif //UNTITLED_GENETIC_ALGORITHM_H
