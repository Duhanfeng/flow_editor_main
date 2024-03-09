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
#include <QRandomGenerator>

//产生随机字符串
QString generateRandomString(int min_length, int max_length)
{
    //确定字符串长度
    int length = QRandomGenerator::global()->bounded(min_length, max_length + 1);
    //预设的字符集，可根据需要修改
    const QString char_pool = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    QString random_string;
    random_string.reserve(length);
    for (int i = 0; i < length; ++i)
    {
        //从字符集中随机选择一个字符
        int index = QRandomGenerator::global()->bounded(char_pool.length());
        QChar next_char = char_pool.at(index);
        random_string.append(next_char);
    }
    return random_string;
}

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    fe::FlowScene* scene = new fe::FlowScene();
    fe::FlowView* flow_view = new fe::FlowView(scene);

    //绘画对象
    std::vector<fe::NodeData> nodes;
    fe::NodeData data1 = {
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
    fe::NodeData data3 = {
        //fe::NodeType::InNode,
        u8"这是一段比较长的标题#1",
        QIcon(),
        { 400.0, 100.0 },
        { { u8"输入端口1", QColor(100, 200, 100) },
            { u8"输入端口2", QColor(100, 200, 100) },
            { u8"输入端口3", QColor(100, 200, 100) } },
        { { u8"输出端口1", QColor(200, 100, 100) },
            { u8"输出端口2", QColor(200, 100, 100) },
            { u8"输出端口3", QColor(200, 100, 100) },
            { u8"输出端口4", QColor(200, 100, 100) } }
    };
    fe::NodeData data4 = {
        //fe::NodeType::InNode,
        u8"节点2",
        QIcon(),
        { 400.0, 300.0 },
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

    nodes.emplace_back(data1);
    nodes.emplace_back(data2);
    nodes.emplace_back(data3);
    nodes.emplace_back(data4);

    //随机生成100个节点
    const int count = 200;
    for (int i = 0; i < count; ++i)
    {
        fe::NodeData random_data;
        //random_data.node_type = fe::NodeType::Node;
        random_data.node_name = generateRandomString(2, 10);
        random_data.position.setX((double)QRandomGenerator::global()->bounded(-500, 500));
        random_data.position.setY((double)QRandomGenerator::global()->bounded(-500, 500));

        int in_count = QRandomGenerator::global()->bounded(1, 5);
        for (int j = 0; j < in_count; ++j)
        {
            fe::PortData port;
            port.port_name = generateRandomString(2, 5);
            random_data.in_port.emplace_back(port);
        }

        int out_count = QRandomGenerator::global()->bounded(1, 5);
        for (int j = 0; j < out_count; ++j)
        {
            fe::PortData port;
            port.port_name = generateRandomString(2, 5);
            random_data.out_port.emplace_back(port);
        }

        nodes.emplace_back(random_data);
    }

    flow_view->resize(1200, 800);
    flow_view->show();

    scene->showNodes(nodes);

    app.exec();
    return 0;
}
