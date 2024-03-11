//
// Created by cxc on 2024/3/11.
//

#pragma once
#include <QGraphicsItem>
#include <flow_editor/flow_view/flow_scene.hpp>
#include <src/items/connection/connection_item.hpp>

namespace fe
{
class DraftConnectionItem;
class AbsNodeItem : public QGraphicsItem
{
public:
    AbsNodeItem(FlowScene& scene, const guid16& id) :
        scene_(scene), id_(id) {}

    const guid16& id() const { return id_; }

    //DraftConnectionItem交互
    DraftConnectionItem const* connectionForReaction() const { return draft_connection_item_; }
    void storeConnectionForReaction(DraftConnectionItem const* item) { draft_connection_item_ = item; }
    void resetConnectionForReaction() { draft_connection_item_ = nullptr; }

protected:
    void moveConnections() const
    {
        std::vector<ConnectionItem*> items = scene_.allConnectionItems(id_);
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
    const DraftConnectionItem* draft_connection_item_ = nullptr;
};
} //namespace fe
