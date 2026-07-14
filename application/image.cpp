//
// Created by fengxuegt on 2026/7/14.
//

#include "image.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Image * Image::CreateImage(const std::string &path) {
    int width, height, sourceChannels;
    stbi_set_flip_vertically_on_load(true);
    auto data = stbi_load(path.c_str(), &width, &height, &sourceChannels, STBI_rgb_alpha);
    // channels是图片实际的通道数，不是返回的通道数，返回的通道数是最后一个参数，不管你是几个通道，都给我返回四个通道！
    if (data) {
        for (int i = 0; i < width * height * 4; i += 4) { // 这里不要使用sizeof(RGBA)，直接使用通道数
            // RGBA   ---->   BGRA     B   and   R   swap
            std::swap(data[i], data[i + 2]);
        }
    } else {
        std::cerr << "Failed to load image: " << path << std::endl;
        return nullptr;
    }
    Image * image = new Image(width, height, STBI_rgb_alpha, reinterpret_cast<RGBA*>(data));
    stbi_image_free(data);
    return image;
}

Image::Image() {
}

Image::Image(int width, int height, int channels, RGBA *data) {
    mWidth = width;
    mHeight = height;
    mChannels = channels;
    if (data) {
        mData = new RGBA[mWidth * mHeight];
        memcpy(mData, data, mWidth * mHeight * sizeof(RGBA));
    }
}

Image::~Image() {
    if (mData) {
        delete[] mData;
    }
}
