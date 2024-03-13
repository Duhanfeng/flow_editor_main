//
// Created by cxc on 2024/3/7.
//

#pragma once
#include <memory>
#include <QGraphicsScene>
#include <flow_editor/data_def.hpp>
#include <flow_editor/flow.hpp>

namespace fe
{
class FlowSceneData;
class SceneConfig;
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
    std::shared_ptr<TypeColorMap> typeColorMap() const;
    void setTypeColorMap(std::shared_ptr<TypeColorMap> map);

    //查询接口
    FlowSceneData* flowSceneData();
    SceneConfig& sceneConfig();

private:
    class Data;
    Data* data_ = nullptr;
};

} //namespace fe
