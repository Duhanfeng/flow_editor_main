//
// Created by cxc on 2024/3/7.
//

#pragma once
#include <QGraphicsScene>
#include <flow_editor/data_def.hpp>
#include <memory>

namespace fe
{
class FlowSceneData;
class FlowScene : public QGraphicsScene
{
public:
    FlowScene(QObject* parent = nullptr);
    ~FlowScene();

    //渲染控制
    void showFlow(std::shared_ptr<Flow> flow);
    std::shared_ptr<Flow> flow();
    void clearItem();

    //样式控制
    NodeLayoutStyle nodeLayoutStyle() const;
    void setNodeLayoutStyle(NodeLayoutStyle node_style);
    std::shared_ptr<NodeStyle> nodeStyle() const;
    void setNodeStyle(std::shared_ptr<NodeStyle> style);

    //查询接口
    FlowSceneData* flowSceneData();

private:
    class Data;
    Data* data_ = nullptr;
};

} //namespace fe
