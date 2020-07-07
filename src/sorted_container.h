//
// Created by DamianPC on 7/7/2020.
//

#ifndef UNTITLED_SORTED_CONTAINER_H
#define UNTITLED_SORTED_CONTAINER_H

#include <vector>

template<class T>
class SortedContainer{
public:
    SortedContainer(unsigned int size){
        container.reserve(size);
    }

    SortedContainer(){}

    void add(T& value){
        container.push_back(value);
    }
    T& get(int i){
        return container.at(i);
    }

    void sort(){
        std::sort(container.begin(), container.end());
    }

    void remove(int i){
        container.erase(container.begin() + i);
    }
private:
    std::vector<T> container;
};

#endif //UNTITLED_SORTED_CONTAINER_H
