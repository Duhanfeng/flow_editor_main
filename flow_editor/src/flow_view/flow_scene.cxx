//
// Created by cxc on 2024/3/7.
//

#include <flow_editor/flow_view/flow_scene.hpp>
#include <src/node_sub_geometry/hor_geometry_compute.hpp>
#include <src/items/node_item.hpp>
#include <src/items/in_node_item.hpp>
#include <src/items/out_node_item.hpp>
#include <QFontDatabase>
#include <iostream>

namespace fe
{
class FlowScene::Data
{
public:
    NodeLayoutStyle node_layout_style = NodeLayoutStyle::Horizontal;
    std::shared_ptr<NodeStyle> node_style;
    QFont def_font;
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
    data_->def_font = QFont(data_->node_style->font_name);
}
FlowScene::~FlowScene()
{
    delete data_;
    data_ = nullptr;
}

void FlowScene::showNodes(const std::vector<NodeData>& nodes)
{
    clearItem();
    if (data_->node_layout_style == NodeLayoutStyle::Horizontal)
    {
        for (size_t i = 0; i < nodes.size(); ++i)
        {
            const auto& node_data = nodes[i];
            if (node_data.node_type == NodeType::Node)
            {
                auto item = new NodeItem(node_data, data_->node_style);
                item->setZValue(i);
                addItem(item);
            }
            else if (node_data.node_type == NodeType::InNode)
            {
                auto item = new InNodeItem(node_data, data_->node_style);
                item->setZValue(i);
                addItem(item);
            }
            else if (node_data.node_type == NodeType::OutNode)
            {
                auto item = new OutNodeItem(node_data, data_->node_style);
                item->setZValue(i);
                addItem(item);
            }
        }
    }
}
void FlowScene::clearItem()
{
    clear();
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