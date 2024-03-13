//
// Created by cxc on 2024/3/13.
//

#pragma once
#include <flow_editor/flow_editor.hpp>
#include <QRandomGenerator>

inline fe::guid16 createGuid16()
{
    fe::guid16 guid;
    for (size_t i = 0; i < guid.size(); ++i)
    {
        guid[i] = QRandomGenerator::global()->bounded(0, 0xFF);
    }
    return guid;
}
inline fe::guid18 createGuid18()
{
    fe::guid18 guid;
    for (size_t i = 0; i < guid.size(); ++i)
    {
        guid[i] = QRandomGenerator::global()->bounded(0, 0xFF);
    }
    return guid;
}