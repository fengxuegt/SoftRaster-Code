//
// Created by fengxuegt on 2026/7/10.
//

#include "gpu.h"

#include "raster.h"

GPU* GPU::mGPUInstance = nullptr;
GPU * GPU::GetInstance() {
    if (mGPUInstance == nullptr) {
        mGPUInstance = new GPU();
    }
    return mGPUInstance;
}

GPU::GPU() {
}

GPU::~GPU() {
    if (mFrameBuffer) {
        delete mFrameBuffer;
    }
}

void GPU::InitSurfaceView(const uint32_t &width, const uint32_t &height, void *data) {
    mFrameBuffer = new FrameBuffer(width, height, data);
}

void GPU::ClearSurfaceView() {
    uint32_t pixelSize = mFrameBuffer->GetWidth() * mFrameBuffer->GetHeight();
    std::fill_n(mFrameBuffer->GetColorBuffer(), pixelSize, RGBA{0, 0, 0, 0}); // 记忆一下这个函数的用法，
}

void GPU::DrawPoint(const uint32_t i, const uint32_t j, const RGBA &color) {
    uint32_t pixelPos = j * mFrameBuffer->GetWidth() + i;
    mFrameBuffer->GetColorBuffer()[pixelPos] = color;
}

void GPU::DrawLine(const Point &p, const Point &q) {
    std::vector<Point> pixels;
    Raster::DrawLine(pixels, p, q);
    for (int i = 0; i < pixels.size(); ++i) {
        MALEOON->DrawPoint(pixels[i].x, pixels[i].y, pixels[i].color);
    }
}

