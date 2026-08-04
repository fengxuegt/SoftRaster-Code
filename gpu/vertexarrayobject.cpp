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
        iter = mBindingMap.insert(std::make_pair(binding,BindingDescription)).first;
    }
    auto &bindingDescription = iter->second;
    bindingDescription.vboID = vboID;
    bindingDescription.itemSize = itemSize;
    bindingDescription.stride = stride;
    bindingDescription.offset = offset;
}
