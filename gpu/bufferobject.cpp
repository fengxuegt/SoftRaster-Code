//
// Created by fengxuegt on 2026/7/31.
//

#include "bufferobject.h"

BufferObject::BufferObject() {

}

BufferObject::~BufferObject() {
    if (mBuffer) {
        delete[] mBuffer;
    }
}

//
void BufferObject::SetBufferData(size_t size, void *data) {
    if (mBuffer != nullptr && mBufferSize < size) {
        delete[] mBuffer;
        mBuffer = nullptr;
    }
    if (mBuffer == nullptr) {
        mBuffer = new byte[size];
        mBufferSize = size;
    }
    memcpy(mBuffer, data, size);
}

byte * BufferObject::GetBuffer() const {
    return mBuffer;
}
