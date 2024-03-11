//
// Created by cxc on 2024/3/7.
//

#pragma once
#include <QGraphicsScene>
#include <flow_editor/data_def.hpp>
#include <memory>

namespace fe
{
class AbsNodeItem;
class ConnectionItem;
class FlowSceneData;
class FlowScene : public QGraphicsScene
{
public:
    FlowScene(QObject* parent = nullptr);
    ~FlowScene();

    //渲染控制
    void showFlow(std::shared_ptr<Flow> flow);
    std::shared_ptr<Flow>& flow();
    void clearItem();

    //样式控制
    NodeLayoutStyle nodeLayoutStyle() const;
    void setNodeLayoutStyle(NodeLayoutStyle node_style);
    std::shared_ptr<NodeStyle> nodeStyle() const;
    void setNodeStyle();

    //查询接口
    FlowSceneData* flowSceneData();
    AbsNodeItem* absNodeItem(const guid16& id);                        //返回当前id对应的AbsNodeItem
    ConnectionItem* connectionItem(const guid18& id);                  //返回当前id对应的ConnectionItem
    std::vector<ConnectionItem*> allConnectionItems(const guid16& id); //返回当前node id所涉及的所有ConnectionItem

    //其他
    void makeDraftConnection(PortType required_port, const guid16& id, unsigned int port_index);
    void resetDraftConnection();

private:
    class Data;
    Data* data_ = nullptr;
};

} //namespace fe
