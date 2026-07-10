#pragma once

#include<iostream>
#include<vector>
#include<map>
#include<cmath>
#include<assert.h>

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
