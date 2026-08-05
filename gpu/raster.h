//
// Created by fengxuegt on 2026/7/13.
//

#ifndef GRAPHIC_RASTER_H
#define GRAPHIC_RASTER_H
#include <complex.h>

#include "datastructure.h"
#include "../global/base.h"
#include "../application/image.h"

class Raster {
public:
    static void DrawLine(std::vector<Point> &results, const Point &p, const Point &q);
    static void InterpolantLine(const Point &p, const Point &q, float weight, Point &target);
    static void InterpolantLine(const VsOutput &p, const VsOutput &q, float weight, VsOutput &target);

    static void DrawTriangle(std::vector<Point> &results, const Point &pointA, const Point &pointB, const Point &pointC);
    static void DrawTriangleReference(std::vector<Point> &results, const Point &pointA, const Point &pointB, const Point &pointC);
    static void InterpolantTriangle(const Point &pointA, const Point &pointB, const Point &pointC, Point &target);

    static RGBA lerpRGBA(const RGBA &a, const RGBA &b, float weight);

public:
    Raster();
    ~Raster();

    static void RasterizeLine(std::vector<VsOutput> & results,
        const VsOutput &p,
        const VsOutput &q);

    static void rasterize(std::vector<VsOutput> & results,
                   const uint32_t & drawMode,
                   const std::vector<VsOutput> & input);

private:

};


#endif //GRAPHIC_RASTER_H