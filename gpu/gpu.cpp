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

uint32_t GPU::GenBuffer() {
    mVBOCount++;
    mVBOMap.insert(std::make_pair(mVBOCount, new BufferObject()));
    return mVBOCount;
}

void GPU::DeleteBuffer(uint32_t &index) {
    auto iter = mVBOMap.find(index);
    if (iter != mVBOMap.end()) {
        delete iter->second;
    }else {
        return;
    }
    mVBOMap.erase(iter);
}

void GPU::BindBuffer(const uint32_t &bufferType, const uint32_t &index) {
    if (bufferType == ARRAY_BUFFER) {
        mCurrentVBO = index;
    } else if (bufferType == ELEMENT_ARRAY_BUFFER) {
        mCurrentEBO = index;
    }
}

void GPU::BufferData(const uint32_t &bufferType, size_t dataSize, void *data) {
    uint32_t bufferID = 0;
    if (bufferType == ARRAY_BUFFER) {
        bufferID = mCurrentVBO;
    } else if (bufferType == ELEMENT_ARRAY_BUFFER) {
        bufferID = mCurrentEBO;
    } else {
        assert(false);
    }
    auto iter = mVBOMap.find(bufferID);
    if (iter != mVBOMap.end()) {
        iter->second->SetBufferData(dataSize, data);
    } else {
        assert(false);
    }
}

uint32_t GPU::GenVertexArray() {
    mVAOCount++;
    mVAOMap.insert(std::make_pair(mVAOCount, new VertexArrayObject()));
    return mVAOCount;
}

void GPU::DeleteVertexArray(uint32_t &index) {
    auto iter = mVAOMap.find(index);
    if (iter != mVAOMap.end()) {
        delete iter->second;
    } else {
        return;
    }
    mVAOMap.erase(iter);
}

void GPU::BindVertexArray(uint32_t index) {
    mCurrentVAO = index;
}

void GPU::VertexAttributePointer(const uint32_t &binding, const uint32_t &itemSize, const uint32_t &stride,
    const uint32_t &offset) {
    auto iter = mVAOMap.find(mCurrentVAO);
    if (iter != mVAOMap.end()) {
        iter->second->Set(binding, mCurrentVBO, itemSize, stride, offset);
    } else {
        assert(false);
    }
}

void GPU::PrintVao(const uint32_t &vaoID) {
    auto iter = mVAOMap.find(vaoID);
    if (iter != mVAOMap.end()) {
        iter->second->Print();
    }
}

void GPU::PerspectiveDivision(VsOutput &vsOutput) {
    float oneOverW = 1.0 / vsOutput.mPosition.w;
    vsOutput.mPosition *= oneOverW;
    vsOutput.mPosition.w = 1.0f;;
}

void GPU::VertexShaderStage(std::vector<VsOutput> &vsOutputs, const VertexArrayObject *vao, const BufferObject *ebo,
    uint32_t first, uint32_t count) {
    auto bindingMap = vao->GetBindingMap();
    byte* indicesData = ebo->GetBuffer();
    uint32_t index = 0;
    for (uint32_t i = first; i < first + count; i++) {
        size_t indicesOffset = i * sizeof(uint32_t);
        memcpy(&index, indicesData + indicesOffset, sizeof(uint32_t));
        VsOutput output = mShader->vertexShader(bindingMap, mVBOMap, index);
        vsOutputs.push_back(output);
    }
}

void GPU::ScreenMapping(VsOutput &value) {
    value.mPosition = mScreenMatrix * value.mPosition;
}

void GPU::DrawElement(const uint32_t &drawMode, const uint32_t &first, const uint32_t &count) {
    if (mCurrentVAO == 0 || mShader == nullptr || count == 0) {
        return;
    }
    auto vaoIter = mVAOMap.find(mCurrentVAO);
    if (vaoIter == mVAOMap.end()) {
        std::cerr << "Can't find VAO" << std::endl;
        return;
    }
    const VertexArrayObject *vao = vaoIter->second;
    auto bindingMap = vao->GetBindingMap();
    auto eboIter = mVBOMap.find(mCurrentEBO);
    if (eboIter == mVBOMap.end()) {
        std::cerr << "Can't find EBO" << std::endl;
        return;
    }
    const BufferObject *ebo = eboIter->second;
    std::vector<VsOutput> vsOutputs;
    VertexShaderStage(vsOutputs, vao, ebo, first, count);
    if (vsOutputs.empty()) {
        return;
    }
    for (auto &output : vsOutputs) {
        PerspectiveDivision(output);
    }
    for (auto &output : vsOutputs) {
        ScreenMapping(output);
    }

    std::vector<VsOutput> rasterOutputs;
    Raster::rasterize(rasterOutputs, drawMode, vsOutputs);
    if (rasterOutputs.empty()) {
        return;
    }
    FsOutput fsOutput;
    uint32_t pixelPos = 0;
    for (uint32_t i = 0; i < rasterOutputs.size(); ++i) {
        mShader->fragmentShader(rasterOutputs[i], fsOutput);
        pixelPos = fsOutput.mPixelPos.y * mFrameBuffer->GetWidth() + fsOutput.mPixelPos.x;
        mFrameBuffer->GetColorBuffer()[pixelPos] = fsOutput.mColor;
    }
}


GPU::~GPU() {
    if (mFrameBuffer) {
        delete mFrameBuffer;
    }
}

void GPU::InitSurfaceView(const uint32_t &width, const uint32_t &height, void *data) {
    mFrameBuffer = new FrameBuffer(width, height, data);
    mScreenMatrix = math::screenMatrix<float>(width - 1, height - 1);
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
            // color = SampleNearest(p.uv);
            color = SampleBilinear(p.uv);
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

RGBA GPU::SampleBilinear(math::vec2f &uv) {
    if (mTexture == nullptr || mTexture->mData == nullptr || mTexture->mWidth <= 0 || mTexture->mHeight <= 0) {
        return RGBA{0, 0, 0, 0};
    }
    float u = std::clamp(uv.x, 0.0f, 1.0f);
    float v = std::clamp(uv.y, 0.0f, 1.0f);
    // 需要保留小数
    float x = u * (mTexture->mWidth - 1);
    float y = v * (mTexture->mHeight - 1);

    // 保留小数之后直接向下取整，就是左上
    int x0 = static_cast<int>(std::floor(x));
    int y0 = static_cast<int>(std::floor(y));
    int x1 = std::min(x0 + 1, mTexture->mWidth - 1);
    int y1 = std::min(y0 + 1, mTexture->mHeight - 1);
    // 左上
    Point a {x0, y1};
    int aPos = y1 * mTexture->mWidth + x0;
    // 左下
    Point b {x0, y0};
    int bPos = y0 * mTexture->mWidth + x0;
    // 右下
    Point c {x1, y0};
    int cPos = y0 * mTexture->mWidth + x1;
    // 右上
    Point d {x1, y1};
    int dPos = y1 * mTexture->mWidth + x1;
    // 先进行纵向插值
    RGBA aColor = mTexture->mData[aPos];
    RGBA bColor = mTexture->mData[bPos];
    RGBA cColor = mTexture->mData[cPos];
    RGBA dColor = mTexture->mData[dPos];

    RGBA leftColor = Raster::lerpRGBA(bColor, aColor, y - y0);
    RGBA rightColor = Raster::lerpRGBA(cColor, dColor, y - y0);
    RGBA result = Raster::lerpRGBA(leftColor, rightColor, x - x0);
    return  result;
}
