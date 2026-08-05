//
// Created by fengxuegt on 2026/8/5.
//

#include "defaultshader.h"

DefaultShader::DefaultShader() {
}

DefaultShader::~DefaultShader() {
}

VsOutput DefaultShader::vertexShader(const std::map<uint32_t, BindingDescription> &bindingMap,
                                     const std::map<uint32_t, BufferObject *> bufferObjectMap, const uint32_t &index) {
    VsOutput output;
    math::vec4f position = GetVector(bindingMap, bufferObjectMap, 0, index);
    position.w = 1.0f;
    math::vec4f color = GetVector(bindingMap, bufferObjectMap, 1, index);
    math::vec2f uv = GetVector(bindingMap, bufferObjectMap, 2, index);

    output.mPosition = mProjectionMatrix * mViewMatrix * mModelMatrix * position;
    output.mColor = color;
    output.mUV = uv;
    return output;
}

void DefaultShader::fragmentShader(const VsOutput &input, FsOutput &output) {
    output.mPixelPos.x = static_cast<int>(input.mPosition.x);
    output.mPixelPos.y = static_cast<int>(input.mPosition.y);
    output.mDepth = input.mPosition.z;
    output.mColor = VectorToRGBA(input.mColor);
}
