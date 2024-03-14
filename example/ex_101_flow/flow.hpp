//
// Created by cxc on 2024/3/13.
//

#pragma once
#include <QMessageBox>
#include <flow_editor/flow_editor.hpp>
#include "util/guid_creator.hpp"
#include "util/random_string.hpp"

class Flow2 : public fe::FlowModel
{
public:
    bool tryDeleteNode(const fe::guid16& id) override
    {
        //删除前确认
        QMessageBox::StandardButton buttons = QMessageBox::question(nullptr, u8"确认", u8"是否要删除对应节点?");
        if (buttons == QMessageBox::StandardButton::Yes)
        {
            return fe::FlowModel::tryDeleteNode(id);
        }
        return false;
    }
    bool tryDeleteItems(const std::vector<fe::guid16>& node_ids, const std::vector<fe::guid18>& connection_ids) override
    {
        //删除前确认
        QMessageBox::StandardButton buttons = QMessageBox::question(nullptr, u8"确认", u8"是否要删除?");
        if (buttons == QMessageBox::StandardButton::Yes)
        {
            return fe::FlowModel::tryDeleteItems(node_ids, connection_ids);
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