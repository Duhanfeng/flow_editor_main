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

    ss::FlowScene* scene = new ss::FlowScene();
    ss::FlowView* flow_view = new ss::FlowView(scene);

    //绘画对象
    std::vector<ss::NodeData> nodes;
    ss::NodeData data = {
        ss::NodeType::Node,
        u8"节点1",
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
    ss::NodeData data2 = {
        ss::NodeType::Node,
        u8"节点2",
        QIcon(),
        { 100.0, 300.0 },
        { { u8"输入端口1", QColor(100, 200, 100) },
            { u8"输入端口2", QColor(100, 200, 100) },
            { u8"输入端口3", QColor(100, 200, 100) } },
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
