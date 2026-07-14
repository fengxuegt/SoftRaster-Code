//
// Created by fengxuegt on 2026/7/10.
//

#ifndef GRAPHIC_GPU_H
#define GRAPHIC_GPU_H
#include <cstdint>

#include "framebuffer.h"
#include "../application/image.h"
#include "../global/base.h"
#define MALEOON GPU::GetInstance()

class GPU {
public:
    // draw函数
    void DrawPoint(const uint32_t i, const uint32_t j, const RGBA &color);
    void DrawLine(const Point &p, const Point &q);
    void DrawTriangle(const Point &pointA, const Point &pointB, const Point &pointC);
    void DrawImage(Image *image);
    void DrawImageWithAlpha(Image *image, const int alpha);

    RGBA SampleNearest(math::vec2f &uv);
    RGBA SampleBilinear(math::vec2f &uv);


public:
    static GPU *GetInstance();
    ~GPU();
    void InitSurfaceView(const uint32_t &width, const uint32_t &height, void* data);
    void ClearSurfaceView();

public:
    void SetEnableBlending(const bool &bEnableBlending);
    void SetTexture(Image *image);

private:
    GPU();

private:
    Point lerp(const Point &a, const Point &b, float weight);

private:
    static GPU *mGPUInstance; // 声明时不能初始化，必须在cpp文件中初始化
    FrameBuffer *mFrameBuffer {nullptr};
    bool mEnableBlending {false};

    Image *mTexture {nullptr};
};


#endif //GRAPHIC_GPU_H
