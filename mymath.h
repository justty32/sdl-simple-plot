#ifndef MY_MATH_H
#define MY_MATH_H

#include <array>

namespace mymath{
    enum SceneType{
        good, same_point, same_line, vertical_line, parallel_line, outside, max
    };

    template<typename T>
    int get_slope(T x1, T x2, T y1, T y2, float& m){
        if (x1 == x2 && y1 != y2) return SceneType::vertical_line;
        if (x1 == x2 && y1 == y2) return SceneType::same_point;
        m = static_cast<double>(y2 - y1) / (x2 - x1);
        return SceneType::good;
    }

    template<typename T>
    int get_equation(T x1, T x2, T y1, T y2, float& a, float& b){
        int result = get_slope(x1,x2,y1,y2,a);
        if (result != SceneType::good) return result;
        b = y1 - x1 * a;
        return SceneType::good;
    }

    template<typename T>
    int get_intersection(float a1, float a2, float b1, float b2, T& x, T& y){
        if (a1 == a2 && b1 == b2) return SceneType::same_line;
        if (a1 == a2 && b1 != b2) return SceneType::parallel_line;
        x = (b2 - b1) / (a1 - a2);
        y = x * a1 + b1;
        return SceneType::good;
    }

    template<typename T>
    T equation_y(float a, float b, T x){ return x * a + b; }

    template<typename T>
    T equation_x(float a, float b, T y){ return static_cast<T>((static_cast<float>(y) - b) / a); }

    template<typename T>
    int get_intersection(std::array<std::array<T, 2>, 4> pts, T& x, T& y){
        int result, result1, result2;
        float a1, a2, b1, b2;
        result1 = get_equation(pts[0][0], pts[1][0], pts[0][1], pts[1][1], a1, b1);
        result2 = get_equation(pts[2][0], pts[3][0], pts[2][1], pts[3][1], a2, b2);
        if (result1 == result2 && result1 == vertical_line){
            if (pts[0][0] == pts[2][0]) return SceneType::same_line;
            else return SceneType::parallel_line;
        }
        if (result1 == result2 && result1 == same_point) return SceneType::same_point; // may be seperate points
        if (result1 == vertical_line){
            if (result2 == same_point){
                if (pts[2][0] == pts[0][0] &&
                    !( (pts[2][1] > pts[0][1] && pts[2][1] > pts[1][1])
                        || (pts[2][1] < pts[0][1] && pts[2][1] < pts[1][1]))){
                            x = pts[2][0]; y = pts[2][1];
                            return SceneType::good;
                        }
                else return SceneType::outside;
            }else{
                T yy = equation_y(a2,b2,pts[0][0]);
                if ((yy > pts[0][1] && yy > pts[1][1]) || (yy < pts[0][1] && yy < pts[1][1]))
                    return SceneType::outside;
                x = pts[0][0]; y = yy;
                return good;
            }
        }else if (result2 == vertical_line){
            if (result1 == same_point){
                if (pts[0][0] == pts[2][0] &&
                    !((pts[0][1] > pts[2][1] && pts[0][1] > pts[3][1])
                        || (pts[0][1] < pts[2][1] && pts[0][1] < pts[3][1]))){
                            x = pts[0][0]; y = pts[0][1];
                            return SceneType::good;
                        }
                else return SceneType::outside;
            }else{
                T yy = equation_y(a1,b1,pts[2][0]);
                if ((yy > pts[2][1] && yy > pts[3][1]) || (yy < pts[2][1] && yy < pts[3][1]))
                    return SceneType::outside;
                x = pts[2][0]; y = yy;
                return good;
            }
        }
        if (result1 == same_point || result2 == same_point){
            if (result1 == same_point){
                if (pts[0][0] * a2 + b2 != pts[0][1])
                    return SceneType::outside; 
                x = pts[0][0]; y = pts[0][1];
            }else{
                if (pts[2][0] * a1 + b1 != pts[2][1])
                    return SceneType::outside;
                x = pts[2][0]; y = pts[2][1];
            }
            return SceneType::good;
        }
        T xx, yy;
        result = get_intersection(a1,a2,b1,b2,xx,yy);
        if (result == parallel_line) return SceneType::parallel_line;
        if (result == same_line) return SceneType::same_line;
        x = xx; y = yy;
        return SceneType::good;
    }

}

#endif //! MY_MATH_H