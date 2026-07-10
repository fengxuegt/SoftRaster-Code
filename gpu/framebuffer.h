//
// Created by fengxuegt on 2026/7/10.
//

#ifndef GRAPHIC_FRAMEBUFFER_H
#define GRAPHIC_FRAMEBUFFER_H
#include <cstdint>

#include "../global/base.h"


class FrameBuffer {
public:
    FrameBuffer();

    FrameBuffer(uint32_t width, uint32_t uint32, void * data);

    ~FrameBuffer();

    uint32_t GetWidth() const { return mWidth; }
    uint32_t GetHeight() const { return mHeight; }
    RGBA* GetColorBuffer() const {return mColorBuffer;  }

private:
    uint32_t mWidth{0};
    uint32_t mHeight{0};

    RGBA *mColorBuffer{nullptr};
    bool mExternBuffer{false};
};


#endif //GRAPHIC_FRAMEBUFFER_H