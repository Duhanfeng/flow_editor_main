//
// Created by cxc on 2024/3/7.
//

#pragma once
#include <QGraphicsView>

namespace fe
{
class FlowScene;
class FlowView final : public QGraphicsView
{
    Q_OBJECT
public:
    FlowView(QWidget* parent);
    FlowView(FlowScene* scene, QWidget* parent = nullptr);
    ~FlowView();

protected:
    void drawBackground(QPainter* painter, const QRectF& r) override;
    void showEvent(QShowEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void closeEvent(QCloseEvent* event) override;
    void enterEvent(QEnterEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void contextMenuEvent(QContextMenuEvent* event) override;

private:
    void scaleUp();
    void scaleDown();
    void setupScale(double scale);

private:
    class Data;
    Data* data_ = nullptr;
};

} //namespace fe
