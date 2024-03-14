//
// Created by cxc on 2024/3/7.
//

#include <flow_editor/flow_view/flow_view.hpp>
#include <flow_editor/flow_view/flow_scene.hpp>
#include <QPainter>
#include <QRectF>
#include <QTimer>
#include <QElapsedTimer>
#include <QShowEvent>
#include <QResizeEvent>
#include <QCloseEvent>
#include <QEnterEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QContextMenuEvent>
#include <iostream>
#include "src/items/node_item.hpp"

namespace fe
{
class FlowView::Data
{
public:
    //测试用
    double fps = 0;
    double time = 0;
    QElapsedTimer timer;

    //当前Scene
    FlowScene* scene = nullptr;

    //配置参数
    double minimum_range = 0;
    double maximum_range = 0;
    double crt_scale = 1.0;
    QPointF click_pos;
    QPointF crt_pos;

    //事件
    QAction* delete_selection_action = nullptr; //删除选择的对象,仅普通node时才生效
    QAction* copy_selection_action = nullptr;   //复制
    QAction* paste_action = nullptr;            //粘贴
};

fe::FlowView::FlowView(QWidget* parent) :
    data_(new Data),
    QGraphicsView(parent)
{
    setDragMode(QGraphicsView::ScrollHandDrag);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setCacheMode(QGraphicsView::CacheBackground);
    setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing, true);
    setOptimizationFlags(QGraphicsView::DontSavePainterState);
    setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setBackgroundBrush(QColor(50, 50, 50));

    data_->minimum_range = 0.15;
    data_->maximum_range = 5;

    int max_size = 32767;
    setSceneRect(-max_size, -max_size, (max_size * 2), (max_size * 2));

    //初始化相关事件
    initAction();

    //启动测试
    data_->timer.start();
    //初始化定时器
    auto* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [&]()
        {
            std::cout << "fps: " << data_->fps << "   time: " << data_->time << " ms" << std::endl;
        });
    timer->start(1000);
}
FlowView::FlowView(FlowScene* scene, QWidget* parent) :
    FlowView(parent)
{
    data_->scene = scene;
    setScene(scene);
}
FlowView::~FlowView()
{
    delete data_;
    data_ = nullptr;
}

void FlowView::drawBackground(QPainter* painter, const QRectF& r)
{
    QGraphicsView::drawBackground(painter, r);
    //return;
    auto draw_grid = [&](double grid_step)
    {
        QRect window_rect = rect();
        QPointF tl = mapToScene(window_rect.topLeft());
        QPointF br = mapToScene(window_rect.bottomRight());

        double left = std::floor(tl.x() / grid_step - 0.5);
        double right = std::floor(br.x() / grid_step + 1.0);
        double bottom = std::floor(tl.y() / grid_step - 0.5);
        double top = std::floor(br.y() / grid_step + 1.0);

        //vertical lines
        for (int xi = int(left); xi <= int(right); ++xi)
        {
            QLineF line(xi * grid_step, bottom * grid_step, xi * grid_step, top * grid_step);

            painter->drawLine(line);
        }

        //horizontal lines
        for (int yi = int(bottom); yi <= int(top); ++yi)
        {
            QLineF line(left * grid_step, yi * grid_step, right * grid_step, yi * grid_step);
            painter->drawLine(line);
        }
    };

    //auto const& flow_view_style = StyleCollection::flowViewStyle();
    QPen pfine(QColor(60, 60, 60), 1.0);
    painter->setPen(pfine);
    draw_grid(15);

    QPen p(QColor(25, 25, 25), 1.0);
    painter->setPen(p);
    draw_grid(150);
}
void FlowView::showEvent(QShowEvent* event)
{
    QGraphicsView::showEvent(event);
}
void FlowView::resizeEvent(QResizeEvent* event)
{
    QGraphicsView::resizeEvent(event);
}
void FlowView::closeEvent(QCloseEvent* event)
{
    QWidget::closeEvent(event);
}
void FlowView::enterEvent(QEnterEvent* event)
{
    QWidget::enterEvent(event);
}
void FlowView::wheelEvent(QWheelEvent* event)
{
    QPoint delta = event->angleDelta();

    if (delta.y() == 0)
    {
        event->ignore();
        return;
    }
    double const d = delta.y() / std::abs(delta.y());
    if (d > 0.0)
    {
        scaleUp();
    }
    else
    {
        scaleDown();
    }
}
void FlowView::keyPressEvent(QKeyEvent* event)
{
    switch (event->key())
    {
    case Qt::Key_Shift:
        setDragMode(QGraphicsView::RubberBandDrag);
        break;
    default:
        break;
    }
    QGraphicsView::keyPressEvent(event);
}
void FlowView::keyReleaseEvent(QKeyEvent* event)
{
    switch (event->key())
    {
    case Qt::Key_Shift:
        setDragMode(QGraphicsView::ScrollHandDrag);
        break;
    default:
        break;
    }
    QGraphicsView::keyReleaseEvent(event);
}
void FlowView::mousePressEvent(QMouseEvent* event)
{
    QGraphicsView::mousePressEvent(event);
    if (event->button() == Qt::LeftButton)
    {
        data_->click_pos = mapToScene(event->pos());
    }
}
void FlowView::mouseMoveEvent(QMouseEvent* event)
{
    QGraphicsView::mouseMoveEvent(event);
    QPointF scene_pos = mapToScene(event->pos());
    data_->crt_pos = scene_pos;

    if (scene()->mouseGrabberItem() == nullptr && event->buttons() == Qt::LeftButton)
    {
        //Make sure shift is not being pressed
        if ((event->modifiers() & Qt::ShiftModifier) == 0)
        {
            QPointF difference = data_->click_pos - scene_pos;
            setSceneRect(sceneRect().translated(difference.x(), difference.y()));
        }
    }
}
void FlowView::mouseReleaseEvent(QMouseEvent* event)
{
    QGraphicsView::mouseReleaseEvent(event);
}
void FlowView::contextMenuEvent(QContextMenuEvent* event)
{
    if (itemAt(event->pos()))
    {
        QGraphicsView::contextMenuEvent(event);
        return;
    }
    //数据校验
    if (!data_->scene || data_->scene->flowModel() == nullptr)
    {
        return;
    }
    const auto scene_pos = mapToScene(event->pos());

    //触发信号
    emit data_->scene->sceneContextMenu(scene_pos);
}

void FlowView::scaleUp()
{
    constexpr double step = 1.2;
    const double factor = std::pow(step, 1.0);

    if (data_->maximum_range > 0)
    {
        QTransform t = transform();
        t.scale(factor, factor);
        if (t.m11() >= data_->maximum_range)
        {
            setupScale(t.m11());
            return;
        }
    }

    scale(factor, factor);
    data_->crt_scale = transform().m11(); //更新当前的缩放系数
}
void FlowView::scaleDown()
{
    constexpr double step = 1.2;
    const double factor = std::pow(step, -1.0);

    if (data_->minimum_range > 0)
    {
        QTransform t = transform();
        t.scale(factor, factor);
        if (t.m11() <= data_->minimum_range)
        {
            setupScale(t.m11());
            return;
        }
    }

    scale(factor, factor);
    data_->crt_scale = transform().m11(); //更新当前的缩放系数
}
void FlowView::setupScale(double scale)
{
    scale = std::max(data_->minimum_range, std::min(data_->maximum_range, scale));

    if (scale <= 0)
    {
        return;
    }
    if (scale == transform().m11())
    {
        return;
    }

    QTransform matrix;
    matrix.scale(scale, scale);
    setTransform(matrix, false);
    data_->crt_scale = scale;
}
void FlowView::paintEvent(QPaintEvent* event)
{
    static long long time = 0;
    static unsigned int fps_count = 0;
    static const unsigned int max_count = 10;

    data_->timer.start();
    QGraphicsView::paintEvent(event);
    time += data_->timer.elapsed();

    //FPS computation
    if (++fps_count == max_count)
    {
        data_->time = time / max_count;
        data_->fps = 1000.0 * max_count / time;
        if (std::isinf(data_->fps))
        {
            data_->fps = 0;
        }
        fps_count = 0;
        time = 0;
    }
}
void FlowView::initAction()
{
    //初始化相关事件

    //删除节点
    data_->delete_selection_action = new QAction(u8"Delete Selection", this);
    data_->delete_selection_action->setShortcutContext(Qt::ShortcutContext::WidgetShortcut);
    data_->delete_selection_action->setShortcut(QKeySequence(QKeySequence::Delete));

    //复制
    data_->copy_selection_action = new QAction("Copy Selection", this);
    data_->copy_selection_action->setShortcutContext(Qt::ShortcutContext::WidgetShortcut);
    data_->copy_selection_action->setShortcut(QKeySequence(QKeySequence::Copy));

    //粘贴
    data_->paste_action = new QAction("Paste Selection", this);
    data_->paste_action->setShortcutContext(Qt::ShortcutContext::WidgetShortcut);
    data_->paste_action->setShortcut(QKeySequence(QKeySequence::Paste));

    //连接相关事件
    connect(data_->delete_selection_action,
        &QAction::triggered,
        this,
        [&]()
        {
            //数据校验
            if (!data_->scene || data_->scene->flowModel() == nullptr)
            {
                return;
            }
            //如果没有操作权限,直接退出
            bool can_remove_node = data_->scene->sceneConfig().flow_permission & FlowPermission::NodeDeletable;
            bool can_remove_connection = data_->scene->sceneConfig().flow_permission & FlowPermission::ConnectionEditable;
            if (!can_remove_node && !can_remove_connection)
            {
                return;
            }

            //提取符合条件的节点
            std::vector<guid16> node_ids;
            std::vector<guid18> connection_ids;
            auto items = data_->scene->selectedItems();
            for (const auto& item : items)
            {
                if (NodeItem* node_item = dynamic_cast<NodeItem*>(item))
                {
                    node_ids.emplace_back(node_item->id());
                }
                else if (ConnectionItem* connection_item = dynamic_cast<ConnectionItem*>(item))
                {
                    connection_ids.emplace_back(connection_item->id());
                }
            }

            if (!can_remove_node)
            {
                node_ids.clear();
            }
            if (!can_remove_connection)
            {
                connection_ids.clear();
            }
            if (node_ids.empty() && connection_ids.empty())
            {
                return;
            }

            //在预览模式中,如果只选中一个connection,则删除所有对应的连接
            bool is_preview = data_->crt_scale < data_->scene->sceneConfig().preview_scale;
            if (is_preview && node_ids.empty() && connection_ids.size() == 1)
            {
                const guid18& connection_id = connection_ids[0];
                //查找所有输入输出匹配的节点
                const auto& connections = data_->scene->flowModel()->connections;
                auto connection_itr = connections.find(connection_id);
                if (connection_itr != connections.end())
                {
                    const auto& out_id = connection_itr->second.out;
                    const auto& in_id = connection_itr->second.in;

                    connection_ids.clear();
                    for (const auto& connection : connections)
                    {
                        if (connection.second.out == out_id && connection.second.in == in_id)
                        {
                            connection_ids.emplace_back(connection.first);
                        }
                    }
                }
            }

            //通知删除对应节点
            if (data_->scene->flowModel()->tryDeleteItems(node_ids, connection_ids))
            {
                QElapsedTimer timer;
                timer.start();
                data_->scene->flowSceneData()->recheck();
                double time_ms = timer.elapsed();
                std::cout << "recheck time: " << time_ms << " ms" << std::endl;
            }
        });
    connect(data_->copy_selection_action,
        &QAction::triggered,
        this,
        [&]()
        {
            //数据校验
            if (!data_->scene || data_->scene->flowModel() == nullptr)
            {
                return;
            }
            //提取符合条件的节点
            std::vector<guid16> node_ids;
            auto items = data_->scene->selectedItems();
            for (const auto& item : items)
            {
                if (NodeItem* node_item = dynamic_cast<NodeItem*>(item))
                {
                    node_ids.emplace_back(node_item->id());
                }
            }
            if (node_ids.empty())
            {
                return;
            }
            //通知复制对应节点
            data_->scene->flowModel()->tryCopyNodes(node_ids);
        });
    connect(data_->paste_action,
        &QAction::triggered,
        this,
        [&]()
        {
            //数据校验
            if (!data_->scene || data_->scene->flowModel() == nullptr)
            {
                return;
            }
            //通知粘贴对应节点
            if (data_->scene->flowModel()->tryPasteNodes(data_->crt_pos.x(), data_->crt_pos.y()))
            {
                data_->scene->flowSceneData()->recheck();
            }
        });

    //注册
    addAction(data_->delete_selection_action);
    addAction(data_->copy_selection_action);
    addAction(data_->paste_action);
}
} //namespace fe