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
    Q_OBJECT
public:
    FlowScene(QObject* parent = nullptr);
    ~FlowScene();

    //渲染控制
    void showFlowModel(std::shared_ptr<FlowModel> flow);
    std::shared_ptr<FlowModel> flowModel();
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

Q_SIGNALS:
    void runBtnClicked(const guid16& node_id);                                     //节点运行按钮点击
    void nodeClicked(const guid16& node_id);                                       //节点单击
    void nodeDoubleClicked(const guid16& node_id);                                 //节点双击
    void nodeContextMenu(const guid16& node_id, const QPointF pos);                //节点右键菜单
    void nodesContextMenu(const std::vector<guid16>& node_ids, const QPointF pos); //节点组右键菜单
    void sceneContextMenu(const QPointF pos);                                      //场景位置菜单

private:
    class Data;
    Data* data_ = nullptr;
};

} //namespace fe
