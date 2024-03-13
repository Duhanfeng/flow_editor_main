#include <iostream>
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
#include <flow_editor/flow_editor.hpp>
#include "util/guid_creator.hpp"
#include "util/random_string.hpp"
#include "util/style_creator.hpp"
#include "flow.hpp"
#include "nodes_creator.hpp"

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
    scene->showFlow(flow);

    //显示
    flow_view->resize(1200, 800);
    flow_view->show();

    return app.exec();
}
