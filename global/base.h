#pragma once

#include<iostream>
#include<vector>
#include<map>
#include<cmath>
#include<assert.h>

#include "../math/vector.h"

#define PI					3.14159265358979323
#define DEG2RAD(theta)		(0.01745329251994329 * (theta))
#define FRACTION(v)			((v) - (int)(v))

using byte = unsigned char;
// 定义了像素类Pixel
struct RGBA {
    byte mB;
    byte mG;
    byte mR;
    byte mA;
    RGBA() {

    }
    RGBA(byte b, byte g, byte r, byte a) {
        mB = b;
        mG = g;
        mR = r;
        mA = a;
    }

};

struct Point {
    Point(){}
    Point(int x, int y):x(x), y(y), color(RGBA{0, 0, 0, 0}) {}
    Point(int x, int y, RGBA color):x(x), y(y), color(color) {}
    int32_t x, y;
    RGBA color;
    math::vec2f uv{0.0f, 0.0f};
};

#define ARRAY_BUFFER 0
#define ELEMENT_ARRAY_BUFFER 1

#define DRAW_LINES 0
#define DRAW_TRIANGLES 1