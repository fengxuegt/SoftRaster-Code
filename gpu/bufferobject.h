//
// Created by fengxuegt on 2026/7/31.
//

#ifndef GRAPHIC_BUFFEROBJECT_H
#define GRAPHIC_BUFFEROBJECT_H
#include "../global/base.h"


class BufferObject {
public:
    BufferObject();
    ~BufferObject();
    BufferObject(const BufferObject&) = delete; // 拷贝构造函数声明为delete

    void SetBufferData(size_t size, void* data);
    byte* GetBuffer() const;

private:
    size_t mBufferSize{0}; // buffer 大小
    void *mBuffer{nullptr}; // 指向Buffer的指针

};


#endif //GRAPHIC_BUFFEROBJECT_H