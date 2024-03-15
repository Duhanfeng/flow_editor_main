//
// Created by cxc on 2024/3/11.
//

#pragma once
#include <QGraphicsItem>
#include <flow_editor/flow_view/flow_scene.hpp>
#include <src/flow_view/flow_scene_data.hpp>
#include <src/items/connection/connection_item.hpp>
#include <src/items/connection/draft_connection_item.hpp>

namespace fe
{
class AbsNodeItem : public QGraphicsItem
{
public:
    AbsNodeItem(FlowScene& scene, const guid16& id) :
        scene_(scene), id_(id) {}
    virtual ~AbsNodeItem() = default;

    const guid16& id() const { return id_; }

    //DraftConnectionItem交互
    DraftConnectionItem const* connectionForReaction() const { return draft_connection_item_; }
    void storeConnectionForReaction(DraftConnectionItem const* item) { draft_connection_item_ = item; }
    void resetConnectionForReaction() { draft_connection_item_ = nullptr; }

    //获取当前位置对应的端口索引(大于等于0: 有效 -1:无效)
    virtual int getPortIndex(PortType required_port, const QPoint& pos) const = 0;

protected:
    void moveConnections()
    {
        std::vector<ConnectionItem*> items = scene_.flowSceneData()->allConnectionItems(id_);
        for (const auto& item : items)
        {
            item->move();
        }
    }
    QVariant itemChange(GraphicsItemChange change, const QVariant& value) override
    {
        if (change == ItemScenePositionHasChanged)
        {
            moveConnections();
        }
        return QGraphicsItem::itemChange(change, value);
    }

protected:
    FlowScene& scene_;
    guid16 id_;
    double preview_scale_ = 0.5;
    FlowPermission flow_permission_ = FlowPermission::FlowPermissionAll; //权限
    const DraftConnectionItem* draft_connection_item_ = nullptr;
};
} //namespace fe
