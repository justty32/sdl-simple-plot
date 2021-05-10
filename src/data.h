#ifndef MY_DATA_H
#define MY_DATA_H

#include <vector>
#include <SDL.h>
#include "util.h"

template<typename T>
struct Data{
    enum Interpolation_type{
        Linear, Constant, Polynomial, _max
    };
    enum Compression_type{
        Abort, Average, max
    };
    T val_min = -200, val_max = 200;
    std::vector<T> vec;
    void restrict_vals();
    void expand(int target_size, Interpolation_type type = Linear);
    void compress(int target_size, Compression_type type = Average);
};

template<typename T>
void Data<T>::restrict_vals(){
    for(int i = 0; i < vec.size(); i++)
        if (vec[i] < val_min) vec[i] = val_min;
        else if (vec[i] > val_max) vec[i] = val_max;
}

template<typename T>
void Data<T>::expand(int target_size, Interpolation_type type){
    if (target_size <= vec.size()) return;
    std::vector<T> nvec(target_size);
    double rate = static_cast<double>(target_size) / vec.size();
    for(int i = 0; i < target_size; i++){
        if (type == Linear){
            int original_floor = static_cast<double>(i) / rate;
            if (original_floor >= vec.size()){
                nvec[i] = vec[vec.size()-1];
                continue;
            }
            double percent_floor = static_cast<double>(
                (i - static_cast<int>(original_floor * rate)) / rate
            );
            nvec[i] = vec[original_floor] * (1 - percent_floor) + vec[original_floor + 1] * percent_floor;
        }else if (type == Constant){
            int original_floor = static_cast<double>(i) / rate;
            if (original_floor >= vec.size()){
                nvec[i] = vec[vec.size()-1];
                continue;
            }
            double percent_floor = static_cast<double>(
                (i - static_cast<int>(original_floor * rate)) / rate
            );
            nvec[i] = percent_floor > 0.5 ? vec[original_floor + 1] : vec[original_floor];
        }
    }
}

template<typename T>
void Data<T>::compress(int target_size, Compression_type type){
    if (target_size >= vec.size()) return;
    std::vector<T> nvec(target_size);
    double rate = static_cast<double>(target_size) / vec.size();
    int reci = static_cast<int>(1.0d / rate);
    for(int i = 0; i < target_size; i++){
        if(type == Average){
            int original= static_cast<int>(static_cast<double>(i) / reci);
            int original_floor = original - reci / 2  >= 0 ? original - reci / 2 : 0;
            int original_top = original + reci / 2  <= vec.size() ? original + reci / 2 : vec.size();
            for(int j = original_floor; j < original_top; j++)
                nvec[i] += vec[j];
            nvec[i] /= original_top - original_floor;
        }else if(type == Constant){
            int original= static_cast<int>(static_cast<double>(i) / reci);
            auto ap = after_point(static_cast<double>(i) / reci);
            if (ap > 0)
                if (original + 1 < vec.size()){
                    nvec[i] = (vec[original] + vec[original + 1]) / 2;
                }
            else if (ap < 0)
                if (original - 1 >= 0){
                    nvec[i] = (vec[original] + vec[original - 1]) / 2;
                }
            else
                nvec[i] = vec[original];
        }
    }
}

#endif // ! MY_DATA_H