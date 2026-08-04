//
// Created by fengxuegt on 2026/8/4.
//

#include "vertexarrayobject.h"

#include <utility>

VertexArrayObject::VertexArrayObject() {
}

VertexArrayObject::~VertexArrayObject() {
}

void VertexArrayObject::Set(uint32_t binding, uint32_t vboID, size_t itemSize, size_t stride, size_t offset) {
    auto iter = mBindingMap.find(binding);
    if (iter == mBindingMap.end()) {
        iter = mBindingMap.insert(std::make_pair(binding,BindingDescription())).first;
    }
    auto &bindingDescription = iter->second;
    bindingDescription.mVBOId = vboID;
    bindingDescription.mItemSize = itemSize;
    bindingDescription.mStride = stride;
    bindingDescription.mOffset = offset;
}

void VertexArrayObject::Print() {
    std::cout << "VertexArrayObject::Print()" << std::endl;
    for (auto &item : mBindingMap) {
        std::cout << "Binding ID:" << item.first << std::endl;
        std::cout << "----VBO ID:" << item.second.mVBOId<< std::endl;
        std::cout << "----ItemSize:" << item.second.mItemSize<< std::endl;
        std::cout << "----Stride:" << item.second.mStride<< std::endl;
        std::cout << "----Offset:" << item.second.mOffset<< std::endl;
    }
}
