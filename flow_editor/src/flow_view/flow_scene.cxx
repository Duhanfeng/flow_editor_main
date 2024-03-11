//
// Created by cxc on 2024/3/7.
//

#include <flow_editor/flow_view/flow_scene.hpp>
#include <src/items/node_item.hpp>
#include <src/items/in_node_item.hpp>
#include <src/items/out_node_item.hpp>
#include <src/items/connection/connection_item.hpp>
#include <QFontDatabase>
#include <iostream>
#include <unordered_map>
#include "flow_scene_data.hpp"

namespace
{
void initDefaultStyle(std::shared_ptr<fe::NodeStyle>& node_style)
{
    if (node_style == nullptr)
    {
        node_style = std::make_shared<fe::NodeStyle>();
    }
    node_style->normal_boundary_color = QColor(255, 255, 255);
    node_style->selected_boundary_color = QColor(255, 165, 0);
    node_style->gradient_color0 = QColor(125, 125, 125);
    node_style->gradient_color1 = QColor(80, 80, 80);
    node_style->gradient_color2 = QColor(64, 64, 64);
    node_style->gradient_color3 = QColor(58, 58, 58);
    node_style->shadow_color = QColor(20, 20, 20);
    node_style->font_color = QColor(255, 255, 255);
    node_style->font_color_faded = QColor(125, 125, 125);
    node_style->connection_point_color = QColor(169, 169, 169);
    node_style->filled_connection_point_color = QColor(0, 255, 255);
    node_style->warning_color = QColor(128, 128, 0);
    node_style->error_color = QColor(255, 0, 0);
    node_style->pen_width = 1.0f;
    node_style->hovered_pen_width = 2.0f;
    node_style->connection_point_diameter = 8.0f;
    node_style->opacity = 1.0f;
    node_style->font_name = u8"NSimSun";
    node_style->font_size = 12;
    node_style->font = QFont(node_style->font_name, node_style->font_size);
}
void initDefaultStyle(std::shared_ptr<fe::ConnectionStyle>& connection_style)
{
    if (connection_style == nullptr)
    {
        connection_style = std::make_shared<fe::ConnectionStyle>();
    }
    connection_style->construction_color = QColor("gray");
    connection_style->normal_color = QColor("darkcyan");
    connection_style->selected_color = QColor(100, 100, 100);
    connection_style->selected_halo_color = QColor("orange");
    connection_style->hovered_color = QColor("lightcyan");
    connection_style->line_width = 3.0f;
    connection_style->construction_line_width = 2.0f;
    connection_style->point_diameter = 10.0f;
    connection_style->use_data_defined_colors = false;
}
} //namespace

namespace fe
{

class FlowScene::Data
{
public:
    NodeLayoutStyle node_layout_style = NodeLayoutStyle::Horizontal;
    std::shared_ptr<NodeStyle> node_style;
    std::shared_ptr<ConnectionStyle> connection_style;

    //深度管理
    double node_z_value = 100;
    double connection_z_value = 0;

    //子组件管理
    std::shared_ptr<Flow> flow;
    std::unique_ptr<FlowSceneData> flow_scene_data; //flow所配套的UI数据
};

FlowScene::FlowScene(QObject* parent) :
    data_(new Data),
    QGraphicsScene(parent)
{
    //必须要显式设置index方法为noindex,否则removeItem再delete时,概率出现崩溃问题
    setItemIndexMethod(QGraphicsScene::NoIndex);

    //初始化默认样式
    initDefaultStyle(data_->node_style);
    initDefaultStyle(data_->connection_style);

    //加载默认字体
    //QFontDatabase::addApplicationFont(":/MiSans-Normal.ttf");
    //data_->node_style->font_name = u8"SimSun";
}
FlowScene::~FlowScene()
{
    delete data_;
    data_ = nullptr;
}

//属性配置
NodeLayoutStyle FlowScene::nodeLayoutStyle() const
{
    return data_->node_layout_style;
}
void FlowScene::setNodeLayoutStyle(NodeLayoutStyle node_style)
{
    data_->node_layout_style = node_style;

    //更新样式
}
std::shared_ptr<NodeStyle> FlowScene::nodeStyle() const
{
    return std::shared_ptr<NodeStyle>();
}
void FlowScene::setNodeStyle()
{
}

//流程控制
void FlowScene::showFlow(std::shared_ptr<Flow> flow)
{
    //清空资源
    clearItem();

    data_->flow = flow;
    if (data_->flow == nullptr)
    {
        return;
    }

    //构建数据
    data_->flow_scene_data = std::make_unique<FlowSceneData>();
    data_->node_z_value = 100;
    for (const auto& node : data_->flow->nodes)
    {
        const auto& guid = node.first;
        const auto& data = node.second;
        //构造NodeItemData对象
        auto item_data = std::make_unique<NodeItemData>();
        item_data->data = data; //直接拷贝NodeData数据
        item_data->geometry = std::make_unique<DynamicHGeometry>(item_data->data, data_->node_style);
        item_data->node_style = data_->node_style;
        item_data->z_value = data_->node_z_value;
        //保存数据
        data_->flow_scene_data->node_items.emplace(guid, std::move(item_data));
        new NodeItem(*this, guid);
        data_->node_z_value++;
    }
    for (const auto& node : data_->flow->in_nodes)
    {
        const auto& guid = node.first;
        const auto& data = node.second;
        //构造InPortItemData对象
        auto item_data = std::make_unique<InPortItemData>();
        item_data->data = data; //直接拷贝NodeData数据
        item_data->geometry = std::make_unique<DynamicHPortGeometry>(NodeType::InNode, item_data->data, data_->node_style);
        item_data->node_style = data_->node_style;
        item_data->z_value = data_->node_z_value;
        //保存数据
        data_->flow_scene_data->in_node_items.emplace(guid, std::move(item_data));
        new InNodeItem(*this, guid);
        data_->node_z_value++;
    }
    for (const auto& node : data_->flow->out_nodes)
    {
        const auto& guid = node.first;
        const auto& data = node.second;
        //构造OutPortItemData对象
        auto item_data = std::make_unique<OutPortItemData>();
        item_data->data = data; //直接拷贝NodeData数据
        item_data->geometry = std::make_unique<DynamicHPortGeometry>(NodeType::OutNode, item_data->data, data_->node_style);
        item_data->node_style = data_->node_style;
        item_data->z_value = data_->node_z_value;
        //保存数据
        data_->flow_scene_data->out_node_items.emplace(guid, std::move(item_data));
        new OutNodeItem(*this, guid);
        data_->node_z_value++;
    }
    data_->connection_z_value = 0;
    for (const auto& connection : data_->flow->connections)
    {
        const auto& guid = connection.first;
        const auto& data = connection.second;
        //构造ConnectionItemData对象
        std::unique_ptr<ConnectionItemData> item_data = std::make_unique<ConnectionItemData>();
        item_data->connection = data;
        item_data->style = data_->connection_style;
        item_data->z_value = data_->connection_z_value;
        //保存数据
        data_->flow_scene_data->connection_items.emplace(guid, std::move(item_data));
        new ConnectionItem(*this, guid);
        data_->connection_z_value++;
    }
}
std::shared_ptr<Flow>& FlowScene::flow()
{
    return data_->flow;
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
AbsNodeItem* FlowScene::absNodeItem(const guid16& id)
{
    if (data_->flow_scene_data)
    {
        auto iter_node = data_->flow_scene_data->node_items.find(id);
        if (iter_node != data_->flow_scene_data->node_items.end())
        {
            return iter_node->second->draw_item;
        }
        auto iter_in = data_->flow_scene_data->in_node_items.find(id);
        if (iter_in != data_->flow_scene_data->in_node_items.end())
        {
            return iter_in->second->draw_item;
        }
        auto iter_out = data_->flow_scene_data->out_node_items.find(id);
        if (iter_out != data_->flow_scene_data->out_node_items.end())
        {
            return iter_out->second->draw_item;
        }
    }
    return nullptr;
}
ConnectionItem* FlowScene::connectionItem(const guid18& id)
{
    if (data_->flow_scene_data)
    {
        auto iter_node = data_->flow_scene_data->connection_items.find(id);
        if (iter_node != data_->flow_scene_data->connection_items.end())
        {
            return iter_node->second->draw_item;
        }
    }
    return nullptr;
}
std::vector<ConnectionItem*> FlowScene::allConnectionItems(const guid16& id)
{
    std::vector<ConnectionItem*> items;
    if (data_->flow_scene_data)
    {
        const auto& connection_items = data_->flow_scene_data->connection_items;
        for (const auto& item : connection_items)
        {
            if ((item.second->connection.in == id) || (item.second->connection.out == id))
            {
                items.emplace_back(item.second->draw_item);
            }
        }
    }

    return items;
}

} //namespace fe