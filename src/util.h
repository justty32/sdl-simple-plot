#ifndef MY_UTIL_H
#define MY_UTIL_H

#include <iostream>
#include <vector>
#include <SDL.h>

void restrict_int(int max, int min, int& num){
    if (num < min) num = min;
    if (num > max) num = max;
}

bool is_in_rect(int x, int y, int minx, int miny, int maxx, int maxy){
    return (x > minx && x < maxx && y > miny && y < maxy);
}

bool is_in_rectwh(int x, int y, int minx, int miny, int w, int h){
    return (x > minx && x < minx + w && y > miny && y < miny + h);
}

template<typename T>
int pn(T n){
    if (n < 0) return -1;
    else return 1;
}

template<typename T>
T after_point(T val){
    if constexpr(std::is_same<T, double>::value || std::is_same<T, float>::value){
        val = abs(val);
        return (val - static_cast<unsigned long long>(val)) * pn(val);
    }else{
        return 0;
    }
}

#endif // ! MY_UTIL_H