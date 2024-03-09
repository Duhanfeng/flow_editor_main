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
    std::shared_ptr<fe::Flow> flow = std::make_shared<fe::Flow>();

    //绘画对象
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
        //fe::NodeType::OutNode,
        u8"这是一段比较长的标题#1",
        QIcon(),
        { 800.0, 100.0 },
        { { u8"输入端口1", QColor(100, 200, 100) },
            { u8"输入端口2", QColor(100, 200, 100) },
            { u8"输入端口3", QColor(100, 200, 100) } },
        { { u8"输出端口1", QColor(200, 100, 100) },
            { u8"输出端口2", QColor(200, 100, 100) },
            { u8"输出端口3", QColor(200, 100, 100) },
            { u8"输出端口4", QColor(200, 100, 100) } }
    };
    fe::NodeData data4 = {
        //fe::NodeType::OutNode,
        u8"节点2",
        QIcon(),
        { 800.0, 300.0 },
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

    fe::NodeData data5 = {
        //fe::NodeType::InNode,
        u8"这是一段比较长的标题#1",
        QIcon(),
        { -800.0, 100.0 },
        { { u8"输入端口1", QColor(100, 200, 100) },
            { u8"输入端口2", QColor(100, 200, 100) },
            { u8"输入端口3", QColor(100, 200, 100) } },
        { { u8"输出端口1", QColor(200, 100, 100) },
            { u8"输出端口2", QColor(200, 100, 100) },
            { u8"输出端口3", QColor(200, 100, 100) },
            { u8"输出端口4", QColor(200, 100, 100) } }
    };
    fe::NodeData data6 = {
        //fe::NodeType::InNode,
        u8"节点2",
        QIcon(),
        { -800.0, 300.0 },
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

    std::array<unsigned char, 16> guid1 = { 01 };
    std::array<unsigned char, 16> guid2 = { 02 };
    std::array<unsigned char, 16> guid3 = { 03 };
    std::array<unsigned char, 16> guid4 = { 04 };
    std::array<unsigned char, 16> guid5 = { 05 };
    std::array<unsigned char, 16> guid6 = { 06 };
    flow->nodes.emplace(guid1, data1);
    flow->nodes.emplace(guid2, data2);
    flow->out_nodes.emplace(guid3, data3);
    flow->out_nodes.emplace(guid4, data4);
    flow->in_nodes.emplace(guid5, data5);
    flow->in_nodes.emplace(guid6, data6);
    std::array<unsigned char, 18> c_guid1 = { 01 };
    flow->connections.emplace(c_guid1, fe::Connection{ guid5, 0, guid3, 0 });

    flow_view->resize(1200, 800);
    flow_view->show();

    scene->showFlow(flow);

    app.exec();
    return 0;
}
