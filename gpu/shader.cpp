//
// Created by fengxuegt on 2026/8/5.
//

#include "shader.h"

Shader::Shader() {
}

Shader::~Shader() {
}

math::vec4f Shader::GetVector(const std::map<uint32_t, BindingDescription> &bindingMap,
                              const std::map<uint32_t, BufferObject *> &bufferObjectMap, const uint32_t &attributeLocation,
                              const uint32_t &index) {
    // 取出本属性的description
    auto bindingIter = bindingMap.find(attributeLocation);
    if (bindingIter == bindingMap.end()) {
        assert(false);
    }
    auto bindingDesc = bindingIter->second;
    auto bufferID = bindingDesc.mVBOId;
    auto vboIter = bufferObjectMap.find(bufferID);
    if (vboIter == bufferObjectMap.end()) {
        assert(false);
    }
    BufferObject *vbo = vboIter->second;
    uint32_t dataOffset = bindingDesc.mStride * index + bindingDesc.mOffset;
    uint32_t dataSize = bindingDesc.mItemSize * sizeof(float);

    const byte* buffer = vbo->GetBuffer() + dataOffset;
    math::vec4f result;
    memcpy(&result, buffer, dataSize);
    return result;
}
