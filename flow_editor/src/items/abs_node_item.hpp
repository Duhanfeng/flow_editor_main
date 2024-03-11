//
// Created by cxc on 2024/3/11.
//

#pragma once
#include <QGraphicsItem>
#include <flow_editor/flow_view/flow_scene.hpp>
#include <src/items/connection/connection_item.hpp>

namespace fe
{
class AbsNodeItem : public QGraphicsItem
{
public:
    AbsNodeItem(FlowScene& scene, const guid16& id) :
        scene_(scene), id_(id) {}

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
};
} //namespace fe
