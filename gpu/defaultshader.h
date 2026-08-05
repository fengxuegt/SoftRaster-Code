//
// Created by fengxuegt on 2026/8/5.
//

#ifndef GRAPHIC_DEFAULTSHADER_H
#define GRAPHIC_DEFAULTSHADER_H
#include "shader.h"


class DefaultShader : public Shader{
public:
    DefaultShader();
    ~DefaultShader();

    VsOutput vertexShader(const std::map<uint32_t, BindingDescription> &bindingMap,
        const std::map<uint32_t, BufferObject *> bufferObjectMap, const uint32_t &index) override;

    void fragmentShader(const VsOutput &input, FsOutput &output) override;

public:
    math ::mat4f mModelMatrix;
    math::mat4f mViewMatrix;
    math::mat4f mProjectionMatrix;
};


#endif //GRAPHIC_DEFAULTSHADER_H