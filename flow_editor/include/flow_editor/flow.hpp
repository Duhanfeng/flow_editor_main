//
// Created by cxc on 2024/3/13.
//

#pragma once

#include <flow_editor/data_def.hpp>

namespace fe
{
class Flow
{
public:
    virtual ~Flow() = default;

public:
    //原始数据
    std::map<guid16, NodeData> nodes;
    std::map<guid16, NodeData> in_nodes;
    std::map<guid16, NodeData> out_nodes;
    std::map<guid18, Connection> connections;

public:
    //界面交互响应
    virtual bool tryDeleteNode(const guid16& id)
    {
        auto iter = nodes.find(id);
        if (iter != nodes.end())
        {
            nodes.erase(iter);
            return true;
        }
        return false;
    }
    virtual bool tryDisconnect(const guid18& id)
    {
        auto iter = connections.find(id);
        if (iter != connections.end())
        {
            connections.erase(iter);
            return true;
        }
        return false;
    }
    virtual guid18 tryConnect(const Connection& connection)
    {
        return { 0 };
    }
};

} //namespace fe