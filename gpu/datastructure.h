
#ifndef GRAPHIC_DATASTRUCTURE_H
#define GRAPHIC_DATASTRUCTURE_H
#include <cstddef>
#include <cstdint>
#include "../math/math.h"
#include "../global/base.h"
struct BindingDescription {
    uint32_t mVBOId{0};
    size_t mItemSize{0};
    size_t mStride{0};
    size_t mOffset{0};
};

struct VsOutput {
    math::vec4f mPosition{0.0f, 0.0f, 0.0f, 1.0f};
    math::vec4f mColor{};
    math::vec2f mUV{0.0f, 0.0f};
};


struct FsOutput {
    math::vec2i mPixelPos{};
    float mDepth;
    RGBA mColor;
};

#endif //GRAPHIC_DATASTRUCTURE_H