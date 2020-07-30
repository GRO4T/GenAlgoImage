//
// Created by DamianPC on 7/7/2020.
//

#ifndef UNTITLED_SORTED_CONTAINER_H
#define UNTITLED_SORTED_CONTAINER_H

#include <vector>

namespace gen_algo_image{
    template<class T>
    class SortedContainer{
    public:
        void Add(T value){
            container.push_back(value);
        }
        T& Get(int i){
            return container[i];
        }

        void Remove(int i){
            container.erase(i);
        }

        void Sort(){
            std::sort(container.begin(), container.end());
        }
    private:
        std::vector<T> container;
    };
}


#endif //UNTITLED_SORTED_CONTAINER_H
