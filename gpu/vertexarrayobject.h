//
// Created by fengxuegt on 2026/8/4.
//

#ifndef GRAPHIC_VERTEXARRAYOBJECT_H
#define GRAPHIC_VERTEXARRAYOBJECT_H
#include "datastructure.h"
#include "bufferobject.h"


class VertexArrayObject {

public:
    VertexArrayObject();
    ~VertexArrayObject();

    void Set(uint32_t binding, uint32_t vboID, size_t itemSize, size_t stride, size_t offset);

    const std::map<uint32_t, BindingDescription>& GetBindingMap() const {
        return mBindingMap;
    }

    void Print();

private:
    std::map<uint32_t, BindingDescription> mBindingMap;

};


#endif //GRAPHIC_VERTEXARRAYOBJECT_H