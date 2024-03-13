//
// Created by cxc on 2024/3/13.
//

#pragma once
#include <flow_editor/flow_editor.hpp>
#include "util/guid_creator.hpp"
#include "util/random_string.hpp"

class Flow2 : public fe::Flow
{
public:
    bool tryDeleteNode(const fe::guid16& id) override
    {
        auto iter = nodes.find(id);
        if (iter != nodes.end())
        {
            nodes.erase(iter);
            return true;
        }
        return false;
    }
    bool tryDisconnect(const fe::guid18& id) override
    {
        auto iter = connections.find(id);
        if (iter != connections.end())
        {
            //std::cout << "connections.erase(iter)" << std::endl;
            connections.erase(iter);
            return true;
        }
        return false;
    }
    fe::guid18 tryConnect(const fe::Connection& connection) override
    {
        auto id = createGuid18();
        connections[id] = connection;
        return id;
    }
};