//
// Created by cxc on 2024/3/7.
//

#include <flow_editor/flow_view/flow_scene.hpp>
#include <src/items/node_item.hpp>
#include <src/node_sub_geometry/hor_geometry_compute.hpp>

namespace ss
{
class FlowScene::Data
{
public:
    NodeStyle node_style;
};

FlowScene::FlowScene(QObject* parent) :
    data_(new Data),
    QGraphicsScene(parent)
{
    //必须要显式设置index方法为noindex,否则removeItem再delete时,概率出现崩溃问题
    setItemIndexMethod(QGraphicsScene::NoIndex);
}
FlowScene::~FlowScene()
{
    delete data_;
    data_ = nullptr;
}

void FlowScene::showNodes(const std::vector<NodeData>& nodes)
{
    clearItem();
    if (data_->node_style == NodeStyle::Horizontal)
    {
        for (const auto& node_data : nodes)
        {
            NodeSubGeometry node_sub_geometry;
            HorGeometryCompute::compute(node_data, node_sub_geometry);
            if (node_data.node_type == NodeType::Node)
            {
                addItem(new NodeItem(node_data, node_sub_geometry));
            }
        }
    }
}
void FlowScene::clearItem()
{
    clear();
}

NodeStyle FlowScene::nodeStyle() const
{
    return data_->node_style;
}
void FlowScene::setNodeStyle(NodeStyle node_style)
{
    data_->node_style = node_style;

    //更新样式
}
} //namespace ss