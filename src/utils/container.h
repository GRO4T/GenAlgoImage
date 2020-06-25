//
// Created by DamianPC on 7/7/2020.
//

#ifndef UNTITLED_CONTAINER_H
#define UNTITLED_CONTAINER_H

#include <vector>

namespace gen_algo_image {
    template<class T>
    class Container {
    public:
        inline void Reserve(int size) {
            container.reserve(size);
        }

        inline void Add(T value) {
            container.push_back(value);
        }

        inline T &Get(int i) {
            return container[i];
        }

        inline void Remove(int i) {
            container.erase(i);
        }

        inline void Sort() {
            std::sort(container.begin(), container.end());
        }

    private:
        std::vector<T> container;
    };
}


#endif //UNTITLED_CONTAINER_H
