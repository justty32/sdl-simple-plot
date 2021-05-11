#ifndef MY_CHART_T
#define MY_CHART_T

#include <SDL.h>
#include <array>
#include "data.h"

struct Object{
    int x = 0, y = 0, w = 800, h = 600;
    bool need_update = false;
    virtual void update(double pass_time);
};

template<typename T>
struct Chart : public Object{
    Data<T>* data = nullptr;
    int x1 = 0, x2 = 1024;
    T y1 = 0, y2 =100;
    
    bool in_rect(SDL_Point point){
        return point.x >= x1 && point.x < x2 && point.y >= y1 && point.y < y2;
    }
    bool in_rect(SDL_Point point, int x_, int y_, int w_, int h_){
        return point.x >= x_ && point.x < x_ + w_ && point.y >= y_ && point.y < y_ + h_;
    }
    bool is_intersect(array<SDL_Point,4> lines, SDL_Point& intersect_p){
        int left1 = 0, left2 = 0, up1 = 0, up2 = 0;
        for(int i = 0; i < lines.size(); i++){
            if (lines[i].x < lines[left1].x) left1 = i;
            if (lines[i].y > lines[up1].y) up1 = i
        }
        for(int i = 0; i < lines.size(); i++){
            if (i != left1 && lines[i].x < lines[left2].x) left2 = i;
            if (i != up1 && lines[i].y > lines[up2].x) up2 = i;
        }
        if ((left1 + left2 == 1 || left1 + left2 == 5)
            ||(up1 + up2 == 1 || up1 + up2 == 5))
            return false;
        // TODO : intersection
    }
    void transform(SDL_Point& point){
        if (!in_rect(point)) return;
        double xrate = w / (x2-x1), yrate = h / (y2-y1);
        point.x = (point.x - x1) * xrate;
        point.y = (point.y - y1) * yrate;
    }
    bool legal(){
        if (data == nullptr) return false;
        return x1 >= 0 && x2 <= data->vec.size()
            && x1 < data->vec.size() && x2 > 0
            && y1 < data->val_max && y2 > data->val_min;
    }
    std::vector<SDL_Point> get_points(int window_w, int window_h){
        if (!legal()) return {};
        std::vector<SDL_Point> result;
        for(int i = x1, i < x2; i++)
            if (data->vec[i] >= y1 && data->vec[i] < y2){
                // at now, the point is in deal rect
                SDL_Point p = transform(SDL_Point{i, data->vec[i]}); // change to chart pos
                p.x += x; p.y += y; // change to window pos
                if (in_rect(p, 0, 0, window_w, window_h)) 
                    result.push_back(p); // if in window, view it
            }
        return result;
    }
    std::vector<std::array<SDL_Point,2>> get_lines(){
        if (!legal()) return {};
        std::vector<std::array<SDL_Point,2>> result;
        for(int i = x1+1, i < x2; i++){
            T y1 = data->vec[i-1], y2 = data->vec[i];
            
        }
        return result;
    }
    void update(double pass_time);
};

#endif // ! MY_CHART_T