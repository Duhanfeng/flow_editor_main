//
// Created by cxc on 2024/3/7.
//

#include <flow_editor/flow_view/flow_scene.hpp>
#include <src/items/node_item.hpp>
#include <src/items/in_node_item.hpp>
#include <src/items/out_node_item.hpp>
#include <src/items/connection_item.hpp>
#include <QFontDatabase>
#include <iostream>
#include <unordered_map>

namespace fe
{
class NodeItemData
{
public:
    NodeData data;
    std::unique_ptr<DynamicHGeometry> geometry;
    NodeItem* draw_item = nullptr;
};
class InPortItemData
{
public:
    NodeData data;
    std::unique_ptr<DynamicHPortGeometry> geometry;
    InNodeItem* draw_item = nullptr;
};
class OutPortItemData
{
public:
    NodeData data;
    std::unique_ptr<DynamicHPortGeometry> geometry;
    OutNodeItem* draw_item = nullptr;
};

class FlowScene::Data
{
public:
    NodeLayoutStyle node_layout_style = NodeLayoutStyle::Horizontal;
    std::shared_ptr<NodeStyle> node_style;

    //子组件管理
    std::shared_ptr<Flow> flow;
    std::map<guid16, std::unique_ptr<NodeItemData>> node_items;
    std::map<guid16, std::unique_ptr<InPortItemData>> in_node_items;
    std::map<guid16, std::unique_ptr<OutPortItemData>> out_node_items;
    std::map<guid18, ConnectionItem*> connection_items;
};

namespace
{
    void initDefaultStyle(std::shared_ptr<NodeStyle>& node_style)
    {
        if (node_style == nullptr)
        {
            node_style = std::make_shared<NodeStyle>();
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
} //namespace

FlowScene::FlowScene(QObject* parent) :
    data_(new Data),
    QGraphicsScene(parent)
{
    //必须要显式设置index方法为noindex,否则removeItem再delete时,概率出现崩溃问题
    setItemIndexMethod(QGraphicsScene::NoIndex);

    //初始化默认样式
    initDefaultStyle(data_->node_style);

    //加载默认字体
    //QFontDatabase::addApplicationFont(":/MiSans-Normal.ttf");
    //data_->node_style->font_name = u8"SimSun";
}
FlowScene::~FlowScene()
{
    delete data_;
    data_ = nullptr;
}
void FlowScene::showFlow(std::shared_ptr<Flow> flow)
{
    //清空资源
    clearItem();

    data_->flow = flow;
    if (data_->flow == nullptr)
    {
        return;
    }

    //根据flow内容显示对象
    double crt_z_value = 0;
    for (const auto& node : data_->flow->nodes)
    {
        const auto& guid = node.first;
        const auto& data = node.second;
        std::unique_ptr<NodeItemData> node_item_data = std::make_unique<NodeItemData>();
        node_item_data->data = data; //直接拷贝NodeData数据
        //构造geometry对象
        node_item_data->geometry = std::make_unique<DynamicHGeometry>(node_item_data->data, data_->node_style);
        //构造item对象
        node_item_data->draw_item = new NodeItem(node_item_data->data, *node_item_data->geometry.get(), data_->node_style, crt_z_value);
        //显示
        addItem(node_item_data->draw_item);
        //保存数据
        data_->node_items.emplace(guid, std::move(node_item_data));
        crt_z_value++;
    }
    for (const auto& node : data_->flow->in_nodes)
    {
        const auto& guid = node.first;
        const auto& data = node.second;
        std::unique_ptr<InPortItemData> node_item_data = std::make_unique<InPortItemData>();
        node_item_data->data = data; //直接拷贝NodeData数据
        //构造geometry对象
        node_item_data->geometry = std::make_unique<DynamicHPortGeometry>(NodeType::InNode, node_item_data->data, data_->node_style);
        //构造item对象
        node_item_data->draw_item = new InNodeItem(node_item_data->data, *node_item_data->geometry.get(), data_->node_style, crt_z_value);
        //显示
        addItem(node_item_data->draw_item);
        //保存数据
        data_->in_node_items.emplace(guid, std::move(node_item_data));
        crt_z_value++;
    }
    for (const auto& node : data_->flow->out_nodes)
    {
        const auto& guid = node.first;
        const auto& data = node.second;
        std::unique_ptr<OutPortItemData> node_item_data = std::make_unique<OutPortItemData>();
        node_item_data->data = data; //直接拷贝NodeData数据
        //构造geometry对象
        node_item_data->geometry = std::make_unique<DynamicHPortGeometry>(NodeType::OutNode, node_item_data->data, data_->node_style);
        //构造item对象
        node_item_data->draw_item = new OutNodeItem(node_item_data->data, *node_item_data->geometry.get(), data_->node_style, crt_z_value);
        //显示
        addItem(node_item_data->draw_item);
        //保存数据
        data_->out_node_items.emplace(guid, std::move(node_item_data));
        crt_z_value++;
    }
}
void FlowScene::showNodes(const std::vector<NodeData>& nodes)
{
    clearItem();
    if (data_->node_layout_style == NodeLayoutStyle::Horizontal)
    {
        for (size_t i = 0; i < nodes.size(); ++i)
        {
            const auto& node_data = nodes[i];
            //if (node_data.node_type == NodeType::Node)
            //{
            //    //auto item = new NodeItem(node_data, data_->node_style);
            //    //item->setZValue(i);
            //    //addItem(item);
            //}
            //else if (node_data.node_type == NodeType::InNode)
            //{
            //    auto item = new InNodeItem(node_data, data_->node_style);
            //    item->setZValue(i);
            //    addItem(item);
            //}
            //else if (node_data.node_type == NodeType::OutNode)
            //{
            //    auto item = new OutNodeItem(node_data, data_->node_style);
            //    item->setZValue(i);
            //    addItem(item);
            //}
        }
    }
}
void FlowScene::clearItem()
{
    clear();
    data_->node_items.clear();
    data_->in_node_items.clear();
    data_->out_node_items.clear();
    data_->connection_items.clear();
}

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

} //namespace fe