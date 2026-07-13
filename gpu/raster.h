//
// Created by fengxuegt on 2026/7/13.
//

#ifndef GRAPHIC_RASTER_H
#define GRAPHIC_RASTER_H
#include "../global/base.h"


class Raster {
public:
    static void DrawLine(std::vector<Point> &results, const Point &p, const Point &q);
    static void InterpolantLine(const Point &p, const Point &q, float weight, Point &target);
public:
    Raster();
    ~Raster();
private:

};


#endif //GRAPHIC_RASTER_H