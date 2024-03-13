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

    //默认样式
    std::shared_ptr<NodeStyle> node_style;
    std::shared_ptr<ConnectionStyle> connection_style;
    std::shared_ptr<DraftConnectionStyle> draft_connection_style;
    //类型映射表
    std::shared_ptr<TypeColorMap> type_color_map; //类型与颜色映射字典
};

FlowScene::FlowScene(QObject* parent) :
    data_(new Data),
    QGraphicsScene(parent)
{
    //必须要显式设置index方法为noindex,否则removeItem再delete时,概率出现崩溃问题
    setItemIndexMethod(QGraphicsScene::NoIndex);

    initDefaultStyle(data_->node_style);
    initDefaultStyle(data_->connection_style);
    initDefaultStyle(data_->draft_connection_style);
    data_->type_color_map = std::make_shared<TypeColorMap>();
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

    //初始化
    data_->flow_scene_data = std::make_unique<FlowSceneData>();
    data_->flow_scene_data->flow = flow;
    data_->flow_scene_data->node_style = data_->node_style;
    data_->flow_scene_data->connection_style = data_->connection_style;
    data_->flow_scene_data->draft_connection_style = data_->draft_connection_style;
    data_->flow_scene_data->type_color_map = data_->type_color_map;
    data_->flow_scene_data->scene = this;
    data_->flow_scene_data->node_z_value = 100;
    data_->flow_scene_data->connection_z_value = 0;

    //构建对象
    for (const auto& node : data_->flow_scene_data->flow->nodes)
    {
        data_->flow_scene_data->addNode(node.first, node.second);
    }
    for (const auto& node : data_->flow_scene_data->flow->in_nodes)
    {
        data_->flow_scene_data->addInNode(node.first, node.second);
    }
    for (const auto& node : data_->flow_scene_data->flow->out_nodes)
    {
        data_->flow_scene_data->addOutNode(node.first, node.second);
    }
    for (const auto& connection : data_->flow_scene_data->flow->connections)
    {
        data_->flow_scene_data->addConnection(connection.first, connection.second);
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
    return data_->node_style;
}
void FlowScene::setNodeStyle(std::shared_ptr<NodeStyle> style)
{
    data_->node_style = style;
}
std::shared_ptr<std::map<QString, QColor>> FlowScene::typeColorMap() const
{
    return data_->type_color_map;
}
void FlowScene::setTypeColorMap(std::shared_ptr<std::map<QString, QColor>> map)
{
    data_->type_color_map = map;
    if (data_->flow_scene_data != nullptr)
    {
        data_->flow_scene_data->type_color_map = data_->type_color_map;
    }
}

} //namespace fe