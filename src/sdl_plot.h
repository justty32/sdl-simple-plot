#ifndef MY_SDL_PLOT_H
#define MY_SDL_PLOT_H

#include <iostream>
#include <vector>
#include <SDL.h>
#include "util.h"
#include "sdl_process.h"

using namespace std;

void draw_line(MainWindow& mw, int x1, int x2, int y1, int y2, int size){
    auto rx = [&](int x){ restrict_int(mw.config.width, 0, x); return x; };
    auto ry = [&](int y){ restrict_int(mw.config.height, 0, y); return y;};
    auto need_draw = [&](){
        return is_in_rect(x1, y1, 0, 0, mw.config.width, mw.config.height)
            && is_in_rect(x2, y2, 0, 0, mw.config.width, mw.config.height);
    };
    if (size <= 0) size = 1;
    for(int i = -size; i < size + 1; i++)
        for(int j = -size; j < size + 1; j++)
            if (need_draw())
            SDL_RenderDrawLine(mw.renderer, rx(x1+i), rx(x2+i), ry(y1+j), ry(y2+j));
}
void draw_point(MainWindow& mw, int x, int y, int size){
    auto rx = [&](int x){ restrict_int(mw.config.width, 0, x); return x; };
    auto ry = [&](int y){ restrict_int(mw.config.height, 0, y); return y;};
    if (size <= 0) size = 1;
    for(int i = -size; i < size + 1; i++)
        for(int j = -size; j < size + 1; j++)
            if (is_in_rect(x, y, 0, 0, mw.config.width, mw.config.height))
                SDL_RenderDrawPoint(mw.renderer, rx(x+i), ry(y+j));
}
void draw_lines(MainWindow& mw, vector<SDL_Point> points, int size){
    for(int i = 1; i < points.size(); i++)
        draw_line(mw, points[i-1].x, points[i].x, points[i-1].y, points[i].y, size);
}

struct Chart{
    int x = 0, y = 0;
    float scalex = 1, scaley = 1;
    int data_absmax = 200;
    int num_data = 4096;
    vector<int> data;
    bool need_update = false;
    void self_adjust(){
        if (scalex <= 0) scalex = 1;
        if (scaley <= 0) scaley = 1;
        if (num_data < 10) num_data = 10;
        if (data.size() < num_data) data.resize(num_data);
        if (data_absmax < 1) data_absmax = 1;
        for(auto& y : data)
            if (y > data_absmax) y = data_absmax;
            else if (y < -data_absmax) y = -data_absmax;
    }
};

bool render_chart(MainWindow& mw, Chart& chart){
	SDL_SetRenderDrawColor( mw.renderer, 0, 255, 0, 255 );
    if (chart.x > mw.config.width - 10) chart.x = mw.config.width - 10;
    if (chart.y > mw.config.height - 10) chart.y = mw.config.height - 10;
    int targetw = chart.scalex * mw.config.width, targeth = chart.scaley * mw.config.height;
    auto ax = [&](int x){ return chart.x + targetw * (float(x) / chart.num_data); };
    auto ay = [&](int y){
        float percent = float(y) / chart.data_absmax;
        int accord = mw.config.height / 2 * (percent + 1);
        return chart.y + chart.scaley * accord; 
    };
    vector<SDL_Point> datas(chart.data.size());
    for(int i = 0; i < datas.size(); i++){
        datas[i].x = ax(i);
        datas[i].y = ay(chart.data[i]);
        if (i > 0 && datas[i-1].x != datas[i].x){
            //draw_point(mw, datas[i].x, datas[i].y, 1);
            //cout << datas[i].x << endl;
        }
    }
	SDL_SetRenderDrawColor( mw.renderer, 0, 0, 255, 255 );
    draw_lines(mw, datas, 1);
    vector<SDL_Point> borders{
        {chart.x, chart.y}, {chart.x, targeth + chart.y}, {chart.x + targetw, chart.y}
        , {chart.x + targetw, chart.y + targeth}
    };
    return false;
}

SDL_Point reflect_chart(MainWindow& mw, Chart& chart, int x, int y){
    int targetw = chart.scalex * mw.config.width, targeth = chart.scaley * mw.config.height;
    if (!is_in_rectwh(x,y,chart.x, chart.y, targetw, targeth)) return {-1,-1};
    return {(float)(x - chart.x) / targetw * chart.num_data
        , ((float)(y - chart.y) / targeth - 1) * chart.data_absmax};
}

bool edit_chart(MainWindow& mw, Chart& chart, int x, int y){
    int targetw = chart.scalex * mw.config.width, targeth = chart.scaley * mw.config.height;
    auto point = reflect_chart(mw, chart, x, y);
    if (point.x == -1) return true;
    x = point.x; y = point.y;
    int effect_width = (1.0f / chart.scalex) / 2;
    if (effect_width == 0) effect_width = 1;
    int effect_start = x - effect_width >= 0 ? x - effect_width : 0;
    int effect_end = x + effect_width <= chart.num_data ? x + effect_width : chart.num_data;
    for(int i = effect_start; i < x; i++){
        float percent = (float)(i - effect_start) / effect_width;
        chart.data[i] = y * percent + chart.data[effect_start] * (1-percent);
    }
    chart.data[x] = y;
    for(int i = x + 1; i < effect_end; i++){
        float percent = (float)(effect_start - i - 1) / effect_width;
        chart.data[i] = y * percent + chart.data[effect_end-1] * (1-percent);
    }
    return false;
}

#endif // ! MY_SDL_PLOT_H