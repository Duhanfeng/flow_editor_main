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
    virtual bool tryDeleteItems(const std::vector<guid16>& node_ids, const std::vector<guid18>& connection_ids)
    {
        //删除nodes
        for (size_t i = 0; i < node_ids.size(); ++i)
        {
            auto iter = nodes.find(node_ids[i]);
            if (iter != nodes.end())
            {
                nodes.erase(iter);
            }
        }
        //删除connections
        for (size_t i = 0; i < connection_ids.size(); ++i)
        {
            auto iter = connections.find(connection_ids[i]);
            if (iter != connections.end())
            {
                connections.erase(iter);
            }
        }
        //删除所有涉及到node的connection
        std::vector<guid18> connection_ids2;
        for (size_t i = 0; i < node_ids.size(); ++i)
        {
            const guid16& node_id = node_ids[i];
            for (const auto& connection : connections)
            {
                if (connection.second.out == node_id || connection.second.in == node_id)
                {
                    connection_ids2.emplace_back(connection.first);
                }
            }
        }
        for (size_t i = 0; i < connection_ids2.size(); ++i)
        {
            auto iter = connections.find(connection_ids2[i]);
            if (iter != connections.end())
            {
                connections.erase(iter);
            }
        }

        return true;
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

    //复制粘贴
    virtual bool tryCopyNodes(const std::vector<guid16>& node_ids)
    {
        return true;
    }
    virtual bool tryPasteNodes(int x, int y)
    {
        return true;
    }
};

} //namespace fe