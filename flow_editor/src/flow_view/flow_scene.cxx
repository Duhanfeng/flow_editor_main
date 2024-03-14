//
// Created by cxc on 2024/3/7.
//

#include <flow_editor/flow_view/flow_scene.hpp>
#include <iostream>
#include <unordered_map>
#include <QFontDatabase>
#include <QAction>
#include <src/flow_view/default_style.hpp>
#include <src/flow_view/flow_scene_data.hpp>
#include <src/items/node_item.hpp>
#include <src/items/in_node_item.hpp>
#include <src/items/out_node_item.hpp>
#include <src/items/connection/connection_item.hpp>
#include <src/items/connection/draft_connection_item.hpp>
#include "scene_config.hpp"

namespace fe
{
class FlowScene::Data
{
public:
    SceneConfig scene_config;

    //flow资源(切换flow后会重建内部资源)
    std::unique_ptr<FlowSceneData> flow_scene_data;
};

FlowScene::FlowScene(QObject* parent) :
    data_(new Data),
    QGraphicsScene(parent)
{
    //必须要显式设置index方法为noindex,否则removeItem再delete时,概率出现崩溃问题
    setItemIndexMethod(QGraphicsScene::NoIndex);

    //初始化对应资源
    initDefaultStyle(data_->scene_config.node_style);
    initDefaultStyle(data_->scene_config.connection_style);
    initDefaultStyle(data_->scene_config.draft_connection_style);
    data_->scene_config.type_color_map = std::make_shared<TypeColorMap>();
    data_->flow_scene_data = std::make_unique<FlowSceneData>(*this, data_->scene_config);
}
FlowScene::~FlowScene()
{
    delete data_;
    data_ = nullptr;
}

//流程控制
void FlowScene::showFlowModel(std::shared_ptr<FlowModel> flow)
{
    //清空资源
    clearItem();
    //初始化
    data_->flow_scene_data->setFlow(flow);
}
std::shared_ptr<FlowModel> FlowScene::flowModel()
{
    if (data_->flow_scene_data != nullptr)
    {
        return data_->flow_scene_data->flow_model;
    }
    return nullptr;
}
void FlowScene::clearItem()
{
    clear();
    data_->flow_scene_data->setFlow(nullptr);
}
FlowSceneData* FlowScene::flowSceneData()
{
    return data_->flow_scene_data.get();
}
SceneConfig& FlowScene::sceneConfig()
{
    return data_->scene_config;
}

//属性配置
NodeLayoutStyle FlowScene::nodeLayoutStyle() const
{
    return data_->scene_config.node_layout_style;
}
void FlowScene::setNodeLayoutStyle(NodeLayoutStyle node_style)
{
    data_->scene_config.node_layout_style = node_style;
}
std::shared_ptr<NodeStyle> FlowScene::nodeStyle() const
{
    return data_->scene_config.node_style;
}
void FlowScene::setNodeStyle(std::shared_ptr<NodeStyle> style)
{
    data_->scene_config.node_style = style;
}
std::shared_ptr<std::map<QString, QColor>> FlowScene::typeColorMap() const
{
    return data_->scene_config.type_color_map;
}
void FlowScene::setTypeColorMap(std::shared_ptr<std::map<QString, QColor>> map)
{
    data_->scene_config.type_color_map = map;
}
} //namespace fe