﻿#include <iostream>
#include <fstream>
#include <istream>
#include <ostream>
#include <sstream>
#include <string>
#include <QApplication>
#include <QMainWindow>
#include <QFile>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMenu>
#include <flow_editor/flow_editor.hpp>
#include "util/guid_creator.hpp"
#include "util/random_string.hpp"
#include "util/style_creator.hpp"
#include "flow.hpp"
#include "nodes_creator.hpp"

void updateConnection(fe::FlowScene* scene)
{
    QObject::connect(scene, &fe::FlowScene::sceneContextMenu, scene, [&](const QPointF pos)
        {
            std::cout << "trigger sceneContextMenu" << std::endl;
            QMenu* menu = new QMenu();
            menu->addAction(new QAction("1"));
            menu->addAction(new QAction("1"));
            menu->addAction(new QAction("1"));
            menu->addAction(new QAction("1"));
            menu->exec(QCursor::pos());
            delete menu;
        });
    QObject::connect(scene, &fe::FlowScene::nodeContextMenu, scene, [&](const fe::guid16& node_id, const QPointF pos)
        {
            std::cout << "trigger sceneContextMenu" << std::endl;
            QMenu* menu = new QMenu();
            menu->addAction(new QAction("2"));
            menu->exec(QCursor::pos());
            delete menu;
        });
    QObject::connect(scene, &fe::FlowScene::nodesContextMenu, scene, [&](const std::vector<fe::guid16>& node_ids, const QPointF pos)
        {
            std::cout << "trigger sceneContextMenu" << std::endl;
            QMenu* menu = new QMenu();
            menu->addAction(new QAction("3"));
            menu->exec(QCursor::pos());
            delete menu;
        });
}

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    //构建对象
    fe::FlowScene* scene = new fe::FlowScene();
    fe::FlowView* flow_view = new fe::FlowView(scene);

    //配置样式
    scene->setNodeStyle(createStyle());
    scene->setTypeColorMap(createTypeColorMap());

    //构建节点
    std::shared_ptr<Flow2> flow = std::make_shared<Flow2>();
    createNodes(flow);
    //createRandomNodes(flow);
    scene->showFlowModel(flow);
    updateConnection(scene);

    flow_view->resize(1200, 800);
    flow_view->show();

    return app.exec();
}
