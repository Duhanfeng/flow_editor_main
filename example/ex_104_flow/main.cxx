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

fe::guid16 createGuid16()
{
    fe::guid16 guid;
    for (size_t i = 0; i < guid.size(); ++i)
    {
        guid[i] = QRandomGenerator::global()->bounded(0, 0xFF);
    }
    return guid;
}
fe::guid18 createGuid18()
{
    fe::guid18 guid;
    for (size_t i = 0; i < guid.size(); ++i)
    {
        guid[i] = QRandomGenerator::global()->bounded(0, 0xFF);
    }
    return guid;
}

class FLow2 : public fe::Flow
{
public:
    bool tryDeleteNode(const fe::guid16& id) override
    {
        auto iter = nodes.find(id);
        if (iter != nodes.end())
        {
            nodes.erase(iter);
            return true;
        }
        return false;
    }
    bool tryDisconnect(const fe::guid18& id) override
    {
        auto iter = connections.find(id);
        if (iter != connections.end())
        {
            std::cout << "connections.erase(iter)" << std::endl;
            connections.erase(iter);
            return true;
        }
        return false;
    }
    fe::guid18 tryConnect(const fe::Connection& connection) override
    {
        auto id = createGuid18();
        connections[id] = connection;
        return id;
    }
};

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

std::shared_ptr<fe::NodeStyle> createStyle()
{
    std::shared_ptr<fe::NodeStyle> node_style = std::make_shared<fe::NodeStyle>();

    node_style->normal_boundary_color = QColor(255, 255, 255);
    node_style->selected_boundary_color = QColor(255, 165, 0);
    node_style->gradient_color0 = QColor(125, 125, 125);
    node_style->gradient_color1 = QColor(80, 80, 80);
    node_style->gradient_color2 = QColor(64, 64, 64);
    node_style->gradient_color3 = QColor(58, 58, 58);
    node_style->shadow_color = QColor(20, 20, 20);
    node_style->font_color = QColor(255, 255, 255);
    node_style->font_color_faded = QColor(125, 125, 125);
    node_style->connection_point_color = QColor(169, 169, 169);
    node_style->filled_connection_point_color = QColor(0, 255, 255);
    node_style->warning_color = QColor(128, 128, 0);
    node_style->error_color = QColor(255, 0, 0);
    node_style->btn_color = QColor(0, 255, 0);
    node_style->btn_hovered_color = QColor(125, 125, 125);
    node_style->btn_checked_color = QColor(0, 125, 0);

    node_style->icon_size = 56.0f;
    node_style->pen_width = 1.0f;
    node_style->hovered_pen_width = 1.0f;
    node_style->connection_point_diameter = 10.0f;
    node_style->opacity = 1.0f;
    node_style->font_name = u8"NSimSun";
    node_style->font_size = 12;
    node_style->font = QFont(node_style->font_name, node_style->font_size);

    //构建映射表
    node_style->type_color_map[u8"整型"] = QColor(0x55, 0xAA, 0xFF);
    node_style->type_color_map[u8"浮点数"] = QColor(0xAA, 0x55, 0x00);
    node_style->type_color_map[u8""] = QColor(0x80, 0x80, 0x80);

    return node_style;
};

void createNodes(std::shared_ptr<FLow2> flow)
{
    QIcon icon(":/icon.bmp");

    //绘画对象
    fe::NodeData data1 = {
        u8"这是一段比较长的标题#1",
        icon,
        { -300.0, 100.0 },
        { { u8"整型1", u8"整型" },
            { u8"浮点数1", u8"浮点数" },
            { u8"输入端口3", u8"" } },
        { { u8"输出端口1", u8"整型" },
            { u8"输出端口2", u8"整型" },
            { u8"输出端口3", u8"浮点数" },
            { u8"输出端口4", u8"" } }
    };
    fe::NodeData data2 = {
        u8"节点2",
        icon,
        { -300.0, 300.0 },
        { { u8"输入端口1", u8"整型" },
            { u8"输入端口2", u8"浮点数" },
            { u8"输入端口2", u8"浮点数" },
            { u8"输入端口2", u8"浮点数" },
            { u8"输入端口2", u8"浮点数" },
            { u8"输入端口2", u8"" },
            { u8"输入端口2", u8"整型" },
            { u8"this is long long port", u8"整型" } },
        { { u8"输出端口1", u8"" },
            { u8"输出端口2", u8"浮点数" },
            { u8"输出端口3", u8"整型" },
            { u8"输出端口4", u8"" } }
    };
    fe::NodeData data3 = {
        u8"这是一段比较长的标题#1",
        icon,
        { 300.0, 100.0 },
        { { u8"输入端口1", u8"整型" },
            { u8"输入端口2", u8"浮点数" },
            { u8"输入端口3", u8"" } },
        { { u8"输出端口1", u8"浮点数" },
            { u8"输出端口2", u8"浮点数" },
            { u8"输出端口3", u8"整型" },
            { u8"输出端口4", u8"" } }
    };
    fe::NodeData data4 = {
        u8"节点2",
        icon,
        { 300.0, 300.0 },
        { { u8"输入端口1", u8"" },
            { u8"输入端口2", u8"整型" },
            { u8"输入端口2", u8"整型" },
            { u8"输入端口2", u8"整型" },
            { u8"输入端口2", u8"浮点数" },
            { u8"输入端口2", u8"浮点数" },
            { u8"输入端口2", u8"" },
            { u8"this is long long port", u8"" } },
        { { u8"输出端口1", u8"浮点数" },
            { u8"输出端口2", u8"浮点数" },
            { u8"输出端口3", u8"整型" },
            { u8"输出端口4", u8"" } }
    };

    fe::NodeData data5 = {
        u8"这是一段比较长的标题#1",
        icon,
        { 0.0, 0.0 },
        { { u8"输入端口1", u8"整型" },
            { u8"输入端口2", u8"" },
            { u8"输入端口3", u8"浮点数" } },
        { { u8"输出端口1", u8"浮点数" },
            { u8"输出端口2", u8"" },
            { u8"输出端口3", u8"浮点数" },
            { u8"输出端口4", u8"" } }
    };
    fe::NodeData data6 = {
        u8"节点2",
        icon,
        { 0.0, 400.0 },
        { { u8"输入端口1", u8"浮点数" },
            { u8"输入端口2", u8"整型" },
            { u8"输入端口2", u8"浮点数" },
            { u8"输入端口2", u8"" },
            { u8"输入端口2", u8"浮点数" },
            { u8"输入端口2", u8"整型" },
            { u8"输入端口2", u8"整型" },
            { u8"this is long long port", u8"" } },
        { { u8"输出端口1", u8"" },
            { u8"输出端口2", u8"整型" },
            { u8"输出端口3", u8"浮点数" },
            { u8"输出端口4", u8"" } }
    };

    std::array<unsigned char, 16> guid1 = { 01 };
    std::array<unsigned char, 16> guid2 = { 02 };
    std::array<unsigned char, 16> guid3 = { 03 };
    std::array<unsigned char, 16> guid4 = { 04 };
    std::array<unsigned char, 16> guid5 = { 05 };
    std::array<unsigned char, 16> guid6 = { 06 };
    flow->in_nodes.emplace(guid1, data1);
    flow->in_nodes.emplace(guid2, data2);
    flow->out_nodes.emplace(guid3, data3);
    flow->out_nodes.emplace(guid4, data4);
    flow->nodes.emplace(guid5, data5);
    flow->nodes.emplace(guid6, data6);
    std::array<unsigned char, 18> c_guid1 = { 01 };
    flow->connections.emplace(c_guid1, fe::Connection{ guid1, 0, guid3, 0 });
}

void createRandomNodes(std::shared_ptr<FLow2> flow)
{
    QIcon icon(":/icon.bmp");
    //随机生成100个节点
    const int count = 0;
    for (int i = 0; i < count; ++i)
    {
        fe::NodeData random_data;
        //random_data.node_type = fe::NodeType::Node;
        random_data.caption_text = generateRandomString(2, 10);
        random_data.position.setX((double)QRandomGenerator::global()->bounded(-2000, 2000));
        random_data.position.setY((double)QRandomGenerator::global()->bounded(-2000, 2000));
        random_data.node_icon = icon;
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
        flow->nodes.emplace(createGuid16(), random_data);
    }
}

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    fe::FlowScene* scene = new fe::FlowScene();
    fe::FlowView* flow_view = new fe::FlowView(scene);
    std::shared_ptr<FLow2> flow = std::make_shared<FLow2>();
    scene->setNodeStyle(createStyle());
    flow_view->resize(1200, 800);
    flow_view->show();

    createNodes(flow);
    scene->showFlow(flow);

    app.exec();
    return 0;
}
