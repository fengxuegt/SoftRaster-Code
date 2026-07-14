//
// Created by fengxuegt on 2026/7/10.
//

#include "gpu.h"

#include "raster.h"

#include <algorithm>
#include <cmath>

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
    if (mFrameBuffer == nullptr || mFrameBuffer->GetColorBuffer() == nullptr) {
        return;
    }

    uint32_t pixelSize = mFrameBuffer->GetWidth() * mFrameBuffer->GetHeight();
    std::fill_n(mFrameBuffer->GetColorBuffer(), pixelSize, RGBA{0, 0, 0, 0}); // 记忆一下这个函数的用法，
}

void GPU::SetEnableBlending(const bool &bEnableBlending) {
    mEnableBlending = bEnableBlending;
}

void GPU::SetTexture(Image *image) {
    mTexture = image;
    if (image) {
        std::cout << image->mWidth << image->mHeight << std::endl;
    }
}

void GPU::DrawPoint(const uint32_t i, const uint32_t j, const RGBA &color) {
    if (mFrameBuffer == nullptr || mFrameBuffer->GetColorBuffer() == nullptr) {
        return;
    }
    if (i >= mFrameBuffer->GetWidth() || j >= mFrameBuffer->GetHeight()) {
        return;
    }

    uint32_t pixelPos = j * mFrameBuffer->GetWidth() + i;
    RGBA result = color;
    if (mEnableBlending) {
        //加入blending
        auto src = color;
        auto dst = mFrameBuffer->GetColorBuffer()[pixelPos];
        float weight = static_cast<float>(src.mA) / 255.0f;

        result.mR = static_cast<float>(src.mR) * weight + static_cast<float>(dst.mR) * (1.0f - weight);
        result.mG = static_cast<float>(src.mG) * weight + static_cast<float>(dst.mG) * (1.0f - weight);
        result.mB = static_cast<float>(src.mB) * weight + static_cast<float>(dst.mB) * (1.0f - weight);
        result.mA = static_cast<float>(src.mA) * weight + static_cast<float>(dst.mA) * (1.0f - weight);
    }

    mFrameBuffer->GetColorBuffer()[pixelPos] = result;
}

void GPU::DrawLine(const Point &p, const Point &q) {
    std::vector<Point> pixels;
    Raster::DrawLine(pixels, p, q);
    for (int i = 0; i < pixels.size(); ++i) {
        MALEOON->DrawPoint(pixels[i].x, pixels[i].y, pixels[i].color);
    }
}

void GPU::DrawTriangle(const Point &pointA, const Point &pointB, const Point &pointC) {
    std::vector<Point> pixels;
    Raster::DrawTriangle(pixels, pointA, pointB, pointC);
    // Raster::DrawTriangleReference(pixels, pointA, pointB, pointC);
    RGBA color;
    for (auto &p : pixels) {
        if (mTexture) {
            color = SampleNearest(p.uv);
        } else {
            color = p.color;
        }
        MALEOON->DrawPoint(p.x, p.y, color);
    }
}

void GPU::DrawImage(Image *image) {
    for (int i = 0; i < image->mWidth; ++i) {
        for (int j = 0; j < image->mHeight; ++j) {
            int currentPos = j * image->mWidth + i;
            MALEOON->DrawPoint(i, j, image->mData[currentPos]);
        }
    }
}

void GPU::DrawImageWithAlpha(Image *image, const int alpha) {
    RGBA color;
    for (uint32_t i = 0; i < image->mWidth; ++i) {
        for (uint32_t j = 0; j < image->mHeight; ++j) {
            color = image->mData[j * image->mWidth + i];
            color.mA = alpha;
            MALEOON->DrawPoint(i, j, color);
        }
    }
}

RGBA GPU::SampleNearest(math::vec2f &uv) {
    if (mTexture == nullptr || mTexture->mData == nullptr || mTexture->mWidth <= 0 || mTexture->mHeight <= 0) {
        return RGBA{0, 0, 0, 0};
    }

    float u = std::clamp(uv.x, 0.0f, 1.0f);
    float v = std::clamp(uv.y, 0.0f, 1.0f);
    int x = static_cast<int>(std::round(u * (mTexture->mWidth - 1)));
    int y = static_cast<int>(std::round(v * (mTexture->mHeight - 1)));
    int currentPos = y * mTexture->mWidth + x;
    return mTexture->mData[currentPos];
}
