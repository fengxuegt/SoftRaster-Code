//
// Created by fengxuegt on 2026/7/14.
//

#ifndef GRAPHIC_IMAGE_H
#define GRAPHIC_IMAGE_H
#include <string>

#include "../global/base.h"


class Image {
public:
    static Image* CreateImage(const std::string &path);
public:
    Image();
    Image(int width, int height, int channels, RGBA *data);
    ~Image();

    // 禁止默认拷贝，否则会发生风险，因为默认拷贝是浅拷贝，容易发生两个对象指向同一块内存；
    Image(const Image&) = delete;
    Image& operator=(const Image&) = delete;
public:
    int mWidth {0};
    int mHeight {0};
    int mChannels {0};
    RGBA *mData {nullptr};

};


#endif //GRAPHIC_IMAGE_H