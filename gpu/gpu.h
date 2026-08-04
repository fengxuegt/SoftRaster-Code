//
// Created by fengxuegt on 2026/7/10.
//

#ifndef GRAPHIC_GPU_H
#define GRAPHIC_GPU_H
#include <cstdint>

#include "bufferobject.h"
#include "framebuffer.h"
#include "vertexarrayobject.h"
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

public:
    uint32_t GenBuffer();
    void DeleteBuffer(uint32_t &index);
    void BindBuffer(const uint32_t &bufferType, const uint32_t &index);
    void BufferData(const uint32_t &bufferType, size_t dataSize, void *data);

    uint32_t GenVertexArray();
    void DeleteVertexArray(uint32_t &index);
    void BindVertexArray(uint32_t index);
    void VertexAttributePointer(
        const uint32_t &binding,
        const uint32_t &itemSize,
        const uint32_t &stride,
        const uint32_t &offset);

    void PrintVao(const uint32_t &vaoID);

private:
    int mVBOCount{0};
    std::map<uint32_t, BufferObject*> mVBOMap;
    int mVAOCount{0};
    std::map<uint32_t, VertexArrayObject*> mVAOMap;

private:
    static GPU *mGPUInstance; // 声明时不能初始化，必须在cpp文件中初始化
    FrameBuffer *mFrameBuffer {nullptr};
    bool mEnableBlending {false};

    Image *mTexture {nullptr};

private:
    uint32_t mCurrentVBO{0};
    uint32_t mCurrentEBO{0};

private:
    uint32_t mCurrentVAO{0};

};


#endif //GRAPHIC_GPU_H
