//
// Created by cxc on 2024/3/7.
//

#include <flow_editor/flow_view/flow_scene.hpp>
#include <src/flow_view/default_style.hpp>
#include <src/flow_view/flow_scene_data.hpp>
#include <src/items/node_item.hpp>
#include <src/items/in_node_item.hpp>
#include <src/items/out_node_item.hpp>
#include <src/items/connection/connection_item.hpp>
#include <src/items/connection/draft_connection_item.hpp>
#include <QFontDatabase>
#include <iostream>
#include <unordered_map>

namespace fe
{
class FlowScene::Data
{
public:
    std::unique_ptr<FlowSceneData> flow_scene_data; //flow所配套的UI数据
};

FlowScene::FlowScene(QObject* parent) :
    data_(new Data),
    QGraphicsScene(parent)
{
    //必须要显式设置index方法为noindex,否则removeItem再delete时,概率出现崩溃问题
    setItemIndexMethod(QGraphicsScene::NoIndex);

    data_->flow_scene_data = std::make_unique<FlowSceneData>();

    //加载默认字体
    //QFontDatabase::addApplicationFont(":/MiSans-Normal.ttf");
    //data_->node_style->font_name = u8"SimSun";
}
FlowScene::~FlowScene()
{
    delete data_;
    data_ = nullptr;
}

//流程控制
void FlowScene::showFlow(std::shared_ptr<Flow> flow)
{
    //清空资源
    clearItem();
    if (flow == nullptr)
    {
        return;
    }

    //构建数据
    data_->flow_scene_data = std::make_unique<FlowSceneData>();
    data_->flow_scene_data->flow = flow;

    initDefaultStyle(data_->flow_scene_data->node_style);
    initDefaultStyle(data_->flow_scene_data->connection_style);
    initDefaultStyle(data_->flow_scene_data->draft_connection_style);
    data_->flow_scene_data->scene = this;

    data_->flow_scene_data->node_z_value = 100;
    for (const auto& node : data_->flow_scene_data->flow->nodes)
    {
        const auto& guid = node.first;
        const auto& data = node.second;
        //构造NodeItemData对象
        auto item_data = std::make_unique<NodeItemData>();
        item_data->data = data; //直接拷贝NodeData数据
        item_data->geometry = std::make_unique<DynamicHGeometry>(item_data->data, data_->flow_scene_data->node_style);
        item_data->node_style = data_->flow_scene_data->node_style;
        item_data->z_value = data_->flow_scene_data->node_z_value;
        //保存数据
        data_->flow_scene_data->node_items.emplace(guid, std::move(item_data));
        new NodeItem(*this, guid);
        data_->flow_scene_data->node_z_value++;
    }
    for (const auto& node : data_->flow_scene_data->flow->in_nodes)
    {
        const auto& guid = node.first;
        const auto& data = node.second;
        //构造InPortItemData对象
        auto item_data = std::make_unique<InPortItemData>();
        item_data->data = data; //直接拷贝NodeData数据
        item_data->geometry = std::make_unique<DynamicHPortGeometry>(NodeType::InNode, item_data->data, data_->flow_scene_data->node_style);
        item_data->node_style = data_->flow_scene_data->node_style;
        item_data->z_value = data_->flow_scene_data->node_z_value;
        //保存数据
        data_->flow_scene_data->in_node_items.emplace(guid, std::move(item_data));
        new InNodeItem(*this, guid);
        data_->flow_scene_data->node_z_value++;
    }
    for (const auto& node : data_->flow_scene_data->flow->out_nodes)
    {
        const auto& guid = node.first;
        const auto& data = node.second;
        //构造OutPortItemData对象
        auto item_data = std::make_unique<OutPortItemData>();
        item_data->data = data; //直接拷贝NodeData数据
        item_data->geometry = std::make_unique<DynamicHPortGeometry>(NodeType::OutNode, item_data->data, data_->flow_scene_data->node_style);
        item_data->node_style = data_->flow_scene_data->node_style;
        item_data->z_value = data_->flow_scene_data->node_z_value;
        //保存数据
        data_->flow_scene_data->out_node_items.emplace(guid, std::move(item_data));
        new OutNodeItem(*this, guid);
        data_->flow_scene_data->node_z_value++;
    }
    data_->flow_scene_data->connection_z_value = 0;
    for (const auto& connection : data_->flow_scene_data->flow->connections)
    {
        const auto& guid = connection.first;
        const auto& data = connection.second;
        data_->flow_scene_data->addConnection(guid, data);
        ////构造ConnectionItemData对象
        //std::unique_ptr<ConnectionItemData> item_data = std::make_unique<ConnectionItemData>();
        //item_data->connection = data;
        //item_data->style = data_->flow_scene_data->connection_style;
        //item_data->z_value = data_->flow_scene_data->connection_z_value;
        ////保存数据
        //data_->flow_scene_data->connection_items.emplace(guid, std::move(item_data));
        //new ConnectionItem(*this, guid);
        //data_->flow_scene_data->connection_z_value++;
    }
}
std::shared_ptr<Flow> FlowScene::flow()
{
    if (data_->flow_scene_data != nullptr)
    {
        return data_->flow_scene_data->flow;
    }
    return nullptr;
}
void FlowScene::clearItem()
{
    clear();
    data_->flow_scene_data = nullptr;
}
FlowSceneData* FlowScene::flowSceneData()
{
    return data_->flow_scene_data.get();
}

//属性配置
NodeLayoutStyle FlowScene::nodeLayoutStyle() const
{
    return data_->flow_scene_data->node_layout_style;
}
void FlowScene::setNodeLayoutStyle(NodeLayoutStyle node_style)
{
    data_->flow_scene_data->node_layout_style = node_style;
}
std::shared_ptr<NodeStyle> FlowScene::nodeStyle() const
{
    return data_->flow_scene_data->node_style;
}
void FlowScene::setNodeStyle(std::shared_ptr<NodeStyle> style)
{
    data_->flow_scene_data->node_style = style;
}

} //namespace fe