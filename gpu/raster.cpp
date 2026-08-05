//
// Created by fengxuegt on 2026/7/13.
//

#include "raster.h"
#include "math.h"
#include "../math/mathfunctions.h"
#include "../math/vector.h"

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

void Raster::InterpolantLine(const VsOutput &start, const VsOutput &end, float weight, VsOutput &target) {
    target.mColor = math::lerp(start.mColor, end.mColor, weight);
    target.mUV = math::lerp(start.mUV, end.mUV, weight);
}

void Raster::DrawTriangle(std::vector<Point> &results, const Point &pointA, const Point &pointB, const Point &pointC) {
    int left = std::min(pointA.x, std::min(pointB.x, pointC.x));
    int right = std::max(pointA.x, std::max(pointB.x, pointC.x));
    int top = std::max(pointA.y, std::max(pointB.y, pointC.y));
    int bottom = std::min(pointA.y, std::min(pointB.y, pointC.y));

    math::vec2f pa, pb, pc;
    Point currentPoint;
    for (int i = left; i <= right; i++) {
        for (int j = bottom; j <= top; j++) {
            currentPoint.x = i;
            currentPoint.y = j;

            pa.x = static_cast<float>(pointA.x - currentPoint.x);
            pa.y = static_cast<float>(pointA.y - currentPoint.y);

            pb.x = static_cast<float>(pointB.x - currentPoint.x);
            pb.y = static_cast<float>(pointB.y - currentPoint.y);

            pc.x = static_cast<float>(pointC.x - currentPoint.x);
            pc.y = static_cast<float>(pointC.y - currentPoint.y);

            // 叉积可以考虑大于或者等于0
            if ((math::cross(pa, pb) >= 0 && math::cross(pb, pc) >= 0 && math::cross(pc, pa) >= 0) ||
                (math::cross(pa, pb) <= 0 && math::cross(pb, pc) <= 0 && math::cross(pc, pa) <= 0)) {
                // 保证在三角形内部再计算插值
                InterpolantTriangle(pointA, pointB, pointC, currentPoint);
                results.push_back(currentPoint);
            }
        }
    }
}

// chatgpt 优化后的代码
void Raster::DrawTriangleReference(std::vector<Point> &results, const Point &pointA, const Point &pointB,
    const Point &pointC) {
        int left = std::min({pointA.x, pointB.x, pointC.x});
        int right = std::max({pointA.x, pointB.x, pointC.x});
        int bottom = std::min({pointA.y, pointB.y, pointC.y});
        int top = std::max({pointA.y, pointB.y, pointC.y});

        for (int x = left; x <= right; ++x) {
            for (int y = bottom; y <= top; ++y) {
                Point currentPoint{};
                currentPoint.x = x;
                currentPoint.y = y;

                // 这种初始化方式值得学习
                math::vec2f pa{
                    static_cast<float>(pointA.x - x),
                    static_cast<float>(pointA.y - y)
                };

                math::vec2f pb{
                    static_cast<float>(pointB.x - x),
                    static_cast<float>(pointB.y - y)
                };

                math::vec2f pc{
                    static_cast<float>(pointC.x - x),
                    static_cast<float>(pointC.y - y)
                };

                // 将结果保存下来减少计算量
                float crossAB = math::cross(pa, pb);
                float crossBC = math::cross(pb, pc);
                float crossCA = math::cross(pc, pa);

                bool hasNegative =
                    crossAB < 0 || crossBC < 0 || crossCA < 0;

                bool hasPositive =
                    crossAB > 0 || crossBC > 0 || crossCA > 0;

                if (!(hasNegative && hasPositive)) {
                    InterpolantTriangle(pointA, pointB, pointC, currentPoint);
                    results.push_back(currentPoint);
                }
            }
        }
}


void Raster::InterpolantTriangle(const Point &pointA, const Point &pointB, const Point &pointC, Point &target) {
    math::vec2f ab {
        static_cast<float>(pointB.x - pointA.x),
        static_cast<float>(pointB.y - pointA.y)
    };
    math::vec2f ac {
        static_cast<float>(pointC.x - pointA.x),
        static_cast<float>(pointC.y - pointA.y)
    };

    math::vec2f pa {
        static_cast<float>(pointA.x - target.x),
        static_cast<float>(pointA.y - target.y)
    };
    math::vec2f pb {
        static_cast<float>(pointB.x - target.x),
        static_cast<float>(pointB.y - target.y)
    };
    math::vec2f pc {
        static_cast<float>(pointC.x - target.x),
        static_cast<float>(pointC.y - target.y)
    };

    float sTriangle = std::abs(math::cross(ab, ac));
    float sPAB = std::abs(math::cross(pa, pb));
    float sPBC = std::abs(math::cross(pb, pc));
    float sPCA = std::abs(math::cross(pc, pa));

    float weightA = sPBC / sTriangle;
    float weightB = sPCA / sTriangle;
    float weightC = sPAB / sTriangle;

    target.color.mA = static_cast<byte>(weightA * pointA.color.mA + weightB * pointB.color.mA + weightC * pointC.color.mA );
    target.color.mB = static_cast<byte>(weightA * pointA.color.mB + weightB * pointB.color.mB + weightC * pointC.color.mB );
    target.color.mR = static_cast<byte>(weightA * pointA.color.mR + weightB * pointB.color.mR + weightC * pointC.color.mR );
    target.color.mG = static_cast<byte>(weightA * pointA.color.mG + weightB * pointB.color.mG + weightC * pointC.color.mG );

    target.uv[0] = static_cast<float>(pointA.uv[0] * weightA + pointB.uv[0] * weightB + pointC.uv[0] * weightC);
    target.uv[1] = static_cast<float>(pointA.uv[1] * weightA + pointB.uv[1] * weightB + pointC.uv[1] * weightC);
}

void Raster::InterpolantTriangle(const VsOutput &pointA, const VsOutput &pointB, const VsOutput &pointC,
    VsOutput &target) {
    math::vec2f ab {
        static_cast<float>(pointB.mPosition.x - pointA.mPosition.x),
        static_cast<float>(pointB.mPosition.y - pointA.mPosition.y)
    };
    math::vec2f ac {
        static_cast<float>(pointC.mPosition.x - pointA.mPosition.x),
        static_cast<float>(pointC.mPosition.y - pointA.mPosition.y)
    };

    math::vec2f pa {
        static_cast<float>(pointA.mPosition.x - target.mPosition.x),
        static_cast<float>(pointA.mPosition.y - target.mPosition.y)
    };
    math::vec2f pb {
        static_cast<float>(pointB.mPosition.x - target.mPosition.x),
        static_cast<float>(pointB.mPosition.y - target.mPosition.y)
    };
    math::vec2f pc {
        static_cast<float>(pointC.mPosition.x - target.mPosition.x),
        static_cast<float>(pointC.mPosition.y - target.mPosition.y)
    };

    float sTriangle = std::abs(math::cross(ab, ac));
    float sPAB = std::abs(math::cross(pa, pb));
    float sPBC = std::abs(math::cross(pb, pc));
    float sPCA = std::abs(math::cross(pc, pa));

    float weightA = sPBC / sTriangle;
    float weightB = sPCA / sTriangle;
    float weightC = sPAB / sTriangle;

    target.mColor = math::lerp(pointA.mColor, pointB.mColor, pointC.mColor, weightA, weightB, weightC);

    target.mUV = math::lerp2(pointA.mUV, pointB.mUV, pointC.mUV, weightA, weightB, weightC);
}

RGBA Raster::lerpRGBA(const RGBA &a, const RGBA &b, float weight) {
    RGBA result;
    result.mA = static_cast<byte>(a.mA * (1 - weight) + b.mA * weight);
    result.mB = static_cast<byte>(a.mB * (1 - weight) + b.mB * weight);
    result.mR = static_cast<byte>(a.mR * (1 - weight) + b.mR * weight);
    result.mG = static_cast<byte>(a.mG * (1 - weight) + b.mG * weight);
    return result;
}


Raster::Raster() {
}

Raster::~Raster() {
}

void Raster::RasterizeLine(std::vector<VsOutput> &results, const VsOutput &p, const VsOutput &q) {
    VsOutput start = p;
    VsOutput end = q;

    // insure x - >
    if (start.mPosition.x > end.mPosition.x) {
        std::swap(start, end);
    }
    results.push_back(start); // 这一步要保证在变换坐标之前，如果在之后的话start就不是原来的start了；

    // insure y ->
    bool flipY = false;
    if (start.mPosition.y > end.mPosition.y) {
        start.mPosition.y *= -1;
        end.mPosition.y *= -1;
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
    if (end.mPosition.x - start.mPosition.x < end.mPosition.y - start.mPosition.y) {
        std::swap(start.mPosition.x, start.mPosition.y);
        std::swap(end.mPosition.x, end.mPosition.y);
        swapXY = true;
    }
    float deltaX = static_cast<float>(end.mPosition.x - start.mPosition.x);
    float deltaY = static_cast<float>(end.mPosition.y - start.mPosition.y);

    // calculate p
    float cur = static_cast<float>(2 * deltaY - deltaX);
    VsOutput curPoint = start;
    int currentX = static_cast<int>(start.mPosition.x);
    int currentY = static_cast<int>(start.mPosition.y);
    for (int i = 0; i < deltaX; i++) {
        currentX++;
        if (cur >=0) {
            currentY++;
            cur -= 2 * deltaX;
        }
        cur += 2 * deltaY;
        curPoint.mPosition.x = currentX;
        curPoint.mPosition.y = currentY;
        float weight = i / static_cast<float>(deltaX);
        InterpolantLine(start, end, weight, curPoint);

        if (swapXY) {
            std::swap(curPoint.mPosition.x, curPoint.mPosition.y);
        }
        if (flipY) {
            curPoint.mPosition.y *= -1;
        }
        // curPoint.color = RGBA(0, 255, 0, 255);
        results.push_back(curPoint);
    }

}

void Raster::RasterizeTriangle(std::vector<VsOutput> &results, const VsOutput &pointA, const VsOutput &pointB,
    const VsOutput &pointC) {
    int left = std::min(pointA.mPosition.x, std::min(pointB.mPosition.x, pointC.mPosition.x));
    int right = std::max(pointA.mPosition.x, std::max(pointB.mPosition.x, pointC.mPosition.x));
    int top = std::max(pointA.mPosition.y, std::max(pointB.mPosition.y, pointC.mPosition.y));
    int bottom = std::min(pointA.mPosition.y, std::min(pointB.mPosition.y, pointC.mPosition.y));

    math::vec2f pa, pb, pc;
    VsOutput currentPoint;
    for (int i = left; i <= right; i++) {
        for (int j = bottom; j <= top; j++) {
            currentPoint.mPosition.x = i;
            currentPoint.mPosition.y = j;

            pa.x = static_cast<float>(pointA.mPosition.x - currentPoint.mPosition.x);
            pa.y = static_cast<float>(pointA.mPosition.y - currentPoint.mPosition.y);

            pb.x = static_cast<float>(pointB.mPosition.x - currentPoint.mPosition.x);
            pb.y = static_cast<float>(pointB.mPosition.y - currentPoint.mPosition.y);

            pc.x = static_cast<float>(pointC.mPosition.x - currentPoint.mPosition.x);
            pc.y = static_cast<float>(pointC.mPosition.y - currentPoint.mPosition.y);

            // 叉积可以考虑大于或者等于0
            if ((math::cross(pa, pb) >= 0 && math::cross(pb, pc) >= 0 && math::cross(pc, pa) >= 0) ||
                (math::cross(pa, pb) <= 0 && math::cross(pb, pc) <= 0 && math::cross(pc, pa) <= 0)) {
                // 保证在三角形内部再计算插值
                InterpolantTriangle(pointA, pointB, pointC, currentPoint);
                results.push_back(currentPoint);
                }
        }
    }
}

void Raster::rasterize(std::vector<VsOutput> &results, const uint32_t &drawMode, const std::vector<VsOutput> &inputs) {
    if (drawMode == DRAW_LINES) {
        for (uint32_t i = 0; i < inputs.size(); i += 2) {
            RasterizeLine(results, inputs[i], inputs[i + 1]);
        }
    }
    if (drawMode == DRAW_TRIANGLES) {
        for (uint32_t i = 0; i < inputs.size(); i += 3) {
            RasterizeTriangle(results, inputs[0], inputs[1], inputs[2]);
        }

    }

}
