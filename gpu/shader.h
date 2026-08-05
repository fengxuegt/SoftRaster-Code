//
// Created by fengxuegt on 2026/8/5.
//

#ifndef GRAPHIC_SHADER_H
#define GRAPHIC_SHADER_H
#include "bufferobject.h"
#include "datastructure.h"


class Shader {
public:
    Shader();
    virtual ~Shader();

    virtual VsOutput vertexShader(
        const std::map<uint32_t, BindingDescription> & bindingMap,
        const std::map<uint32_t, BufferObject*> bufferObjectMap,
        const uint32_t &index
        ) = 0;
    virtual void fragmentShader(
        const VsOutput & input,
        FsOutput & output) = 0;

public:
    // tool functions
    math::vec4f GetVector(
        const std::map<uint32_t, BindingDescription> & bindingMap,
        const std::map<uint32_t, BufferObject*> &bufferObjectMap,
        const uint32_t &attributeLocation,
        const uint32_t &index);
    // 将0-1转换成0-255
    RGBA VectorToRGBA(const math::vec4f &v) {
        RGBA color;
        color.mR = v.x * 255.0f;
        color.mG = v.y * 255.0f;
        color.mB = v.z * 255.0f;
        color.mA = v.w * 255.0f;
        return color;
    }

};


#endif //GRAPHIC_SHADER_H