//
// Created by cxc on 2024/3/7.
//

#pragma once
#include <QGraphicsScene>
#include <flow_editor/data_def.hpp>
#include <memory>

namespace fe
{
class FlowScene : public QGraphicsScene
{
public:
    FlowScene(QObject* parent = nullptr);
    ~FlowScene();

    //渲染控制
    void showNodes(const std::vector<NodeData>& node_data);
    void showFlow(std::shared_ptr<Flow> flow);
    void clearItem();

    //样式控制
    NodeLayoutStyle nodeLayoutStyle() const;
    void setNodeLayoutStyle(NodeLayoutStyle node_style);
    std::shared_ptr<NodeStyle> nodeStyle() const;
    void setNodeStyle();

private:
    class Data;
    Data* data_ = nullptr;
};

} //namespace fe
