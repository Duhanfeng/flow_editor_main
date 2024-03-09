#include <iostream>
#include <fstream>
#include <string>
#include <string>
#include <sstream>
#include <iostream>
#include <istream>
#include <ostream>
#include <QApplication>
#include <QMainWindow>
#include <QFile>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <flow_editor/flow_view/flow_view.hpp>
#include <flow_editor/flow_view/flow_scene.hpp>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    fe::FlowScene* scene = new fe::FlowScene();
    fe::FlowView* flow_view = new fe::FlowView(scene);

    //绘画对象
    std::vector<fe::NodeData> nodes;
    fe::NodeData data = {
        //fe::NodeType::Node,
        u8"这是一段比较长的标题#1",
        QIcon(),
        { 100.0, 100.0 },
        { { u8"输入端口1", QColor(100, 200, 100) },
            { u8"输入端口2", QColor(100, 200, 100) },
            { u8"输入端口3", QColor(100, 200, 100) } },
        { { u8"输出端口1", QColor(200, 100, 100) },
            { u8"输出端口2", QColor(200, 100, 100) },
            { u8"输出端口3", QColor(200, 100, 100) },
            { u8"输出端口4", QColor(200, 100, 100) } }
    };
    fe::NodeData data2 = {
        //fe::NodeType::Node,
        u8"节点2",
        QIcon(),
        { 100.0, 300.0 },
        { { u8"输入端口1", QColor(100, 200, 100) },
            { u8"输入端口2", QColor(100, 200, 100) },
            { u8"输入端口2", QColor(100, 200, 100) },
            { u8"输入端口2", QColor(100, 200, 100) },
            { u8"输入端口2", QColor(100, 200, 100) },
            { u8"输入端口2", QColor(100, 200, 100) },
            { u8"输入端口2", QColor(100, 200, 100) },
            { u8"this is long long port", QColor(100, 200, 100) } },
        { { u8"输出端口1", QColor(200, 100, 100) },
            { u8"输出端口2", QColor(200, 100, 100) },
            { u8"输出端口3", QColor(200, 100, 100) },
            { u8"输出端口4", QColor(200, 100, 100) } }
    };

    nodes.emplace_back(data);
    nodes.emplace_back(data2);

    flow_view->resize(1200, 800);
    flow_view->show();

    scene->showNodes(nodes);

    app.exec();
    return 0;
}
