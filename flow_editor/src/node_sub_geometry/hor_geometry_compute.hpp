﻿//
// Created by cxc on 2024/3/7.
//

#pragma once
#include <flow_editor/data_def.hpp>
#include <src/node_sub_geometry/node_sub_geometry.hpp>

namespace ss
{
class HorGeometryCompute
{
public:
    static void compute(const NodeData& data, NodeSubGeometry& node_sub_geometry);
};

} //namespace ss
