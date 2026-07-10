//
// Created by fengxuegt on 2026/7/10.
//

#include "framebuffer.h"

FrameBuffer::FrameBuffer() {
}

FrameBuffer::FrameBuffer(uint32_t width, uint32_t height, void *data) {
    mWidth = width;
    mHeight = height;
    if (data != nullptr) {
        mExternBuffer = true;
    } else {
        data = new RGBA[mWidth * mHeight];
        mExternBuffer = false;
    }
    mColorBuffer = (RGBA*)data;
}

FrameBuffer::~FrameBuffer() {
    if (!mExternBuffer && mColorBuffer != nullptr) {
        delete[] mColorBuffer;
    }
}
