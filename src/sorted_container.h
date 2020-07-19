//
// Created by DamianPC on 7/7/2020.
//

#ifndef UNTITLED_SORTED_CONTAINER_H
#define UNTITLED_SORTED_CONTAINER_H

#include <vector>

template<class T>
class SortedContainer{
public:
    void add(T value){
        container.push_back(value);
    }
    T& get(int i){
        return container[i];
    }

    void remove(int i){
        container.erase(i);
    }
private:
    std::vector<T> container;
};

#endif //UNTITLED_SORTED_CONTAINER_H
