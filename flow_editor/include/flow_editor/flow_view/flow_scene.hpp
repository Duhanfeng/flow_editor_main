//
// Created by cxc on 2024/3/7.
//

#pragma once
#include <QGraphicsScene>
#include <flow_editor/data_def.hpp>

namespace fe
{
enum class NodeStyle
{
    Horizontal,
    Vertical
};
class FlowScene : public QGraphicsScene
{
public:
    FlowScene(QObject* parent = nullptr);
    ~FlowScene();

    //渲染控制
    void showNodes(const std::vector<NodeData>& node_data);
    void clearItem();

    //样式控制
    NodeStyle nodeStyle() const;
    void setNodeStyle(NodeStyle node_style);

private:
    class Data;
    Data* data_ = nullptr;
};

} //namespace fe
