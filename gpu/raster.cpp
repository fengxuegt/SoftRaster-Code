//
// Created by fengxuegt on 2026/7/13.
//

#include "raster.h"

void Raster::DrawLine(std::vector<Point> &results, const Point &p, const Point &q) {
    Point start = p;
    Point end = q;

    // insure x - >
    if (start.x > end.x) {
        std::swap(start, end);
    }
    results.push_back(start); // 这一步要保证在变换坐标之前，如果在之后的话start就不是原来的start了；

    // insure y ->
    bool flipY = false;
    if (start.y > end.y) {
        start.y *= -1;
        end.y *= -1;
        flipY = true;
    }

    // 写法1
    // 先计算delta，如果要交换xy的话delta也要交换
    // insure rate < 1
    // bool swapXY = false;
    // float deltaX = static_cast<float>(end.x - start.x);
    // float deltaY = static_cast<float>(end.y - start.y);
    // if (deltaX < deltaY) {
    //     std::swap(start.x, start.y);
    //     std::swap(end.x, end.y);
    //     std::swap(deltaX, deltaY); // 这里别忘记了；要不就交换完成之后再计算delta
    //     swapXY = true;
    // }


    // 写法2
    // 首先计算要不要交换xy，然后计算delta
    bool swapXY = false;
    if (end.x - start.x < end.y - start.y) {
        std::swap(start.x, start.y);
        std::swap(end.x, end.y);
        swapXY = true;
    }
    float deltaX = static_cast<float>(end.x - start.x);
    float deltaY = static_cast<float>(end.y - start.y);

    // calculate p
    float cur = static_cast<float>(2 * deltaY - deltaX);
    Point curPoint = start;
    int currentX = static_cast<int>(start.x);
    int currentY = static_cast<int>(start.y);
    for (int i = 0; i < deltaX; i++) {
        currentX++;
        if (cur >=0) {
            currentY++;
            cur -= 2 * deltaX;
        }
        cur += 2 * deltaY;
        curPoint.x = currentX;
        curPoint.y = currentY;
        float weight = i / static_cast<float>(deltaX);
        InterpolantLine(start, end, weight, curPoint);

        if (swapXY) {
            std::swap(curPoint.x, curPoint.y);
        }
        if (flipY) {
            curPoint.y *= -1;
        }
        // curPoint.color = RGBA(0, 255, 0, 255);
        results.push_back(curPoint);
    }


}

void Raster::InterpolantLine(const Point &start, const Point &end, float weight, Point &target) {
    RGBA color;
    color.mA = static_cast<byte>(start.color.mA * (1 - weight) + end.color.mA * weight);
    color.mR = static_cast<byte>(start.color.mR * (1 - weight) + end.color.mR * weight);
    color.mG = static_cast<byte>(start.color.mG * (1 - weight) + end.color.mG * weight);
    color.mB = static_cast<byte>(start.color.mB * (1 - weight) + end.color.mB * weight);
    target.color = color;
}

Raster::Raster() {
}

Raster::~Raster() {
}
