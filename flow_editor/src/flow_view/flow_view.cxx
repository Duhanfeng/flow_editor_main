//
// Created by cxc on 2024/3/7.
//

#include <flow_editor/flow_view/flow_view.hpp>
#include <flow_editor/flow_view/flow_scene.hpp>
#include <QPainter>
#include <QRectF>
#include <QShowEvent>
#include <QResizeEvent>
#include <QCloseEvent>
#include <QEnterEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QContextMenuEvent>

namespace ss
{
class FlowView::Data
{
public:
    double minimum_range = 0;
    double maximum_range = 0;
};

ss::FlowView::FlowView(QWidget* parent) :
    data_(new Data),
    QGraphicsView(parent)
{
    setDragMode(QGraphicsView::ScrollHandDrag);
    setRenderHint(QPainter::Antialiasing); //设置视图的渲染模式为反锯齿
    setBackgroundBrush(QColor(50, 50, 50));
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setCacheMode(QGraphicsView::CacheBackground);
    setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);

    data_->minimum_range = 0.15;
    data_->maximum_range = 2;

    int max_size = 32767;
    setSceneRect(-max_size, -max_size, (max_size * 2), (max_size * 2));
}
FlowView::FlowView(FlowScene* scene, QWidget* parent) :
    FlowView(parent)
{
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
    return;
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
    QGraphicsView::keyPressEvent(event);
}
void FlowView::keyReleaseEvent(QKeyEvent* event)
{
    QGraphicsView::keyReleaseEvent(event);
}
void FlowView::mousePressEvent(QMouseEvent* event)
{
    QGraphicsView::mousePressEvent(event);
}
void FlowView::mouseMoveEvent(QMouseEvent* event)
{
    QGraphicsView::mouseMoveEvent(event);
}
void FlowView::mouseReleaseEvent(QMouseEvent* event)
{
    QGraphicsView::mouseReleaseEvent(event);
}
void FlowView::contextMenuEvent(QContextMenuEvent* event)
{
    QGraphicsView::contextMenuEvent(event);
}

void FlowView::scaleUp()
{
    double const step = 1.2;
    double const factor = std::pow(step, 1.0);

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
    //Q_EMIT scaleChanged(transform().m11());
}
void FlowView::scaleDown()
{
    double const step = 1.2;
    double const factor = std::pow(step, -1.0);

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
    //Q_EMIT scaleChanged(transform().m11());
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

    //Q_EMIT scaleChanged(scale);
}
} //namespace ss