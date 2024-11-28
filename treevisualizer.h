#ifndef TREEVISUALIZER_H
#define TREEVISUALIZER_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QGraphicsLineItem>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <map>
#include "binarysearchtree.h"

class TreeVisualizer : public QGraphicsView {
    Q_OBJECT

public:
    explicit TreeVisualizer(QWidget *parent = nullptr);
    ~TreeVisualizer() override;

    void setBST(const std::shared_ptr<BinarySearchTree>& bst);
    void highlightPath(const std::vector<int>& path, QColor color);
    void clearHighlights();
    void updateTree();

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    struct NodeGraphics {
        QGraphicsEllipseItem* circle = nullptr;
        QGraphicsTextItem* text = nullptr;
        QGraphicsLineItem* parentLine = nullptr;
        QPointF targetPos;
    };

    QGraphicsScene* scene;
    std::shared_ptr<BinarySearchTree> bst;
    std::map<int, NodeGraphics> nodeItems;
    const int NODE_RADIUS = 20;
    const int LEVEL_HEIGHT = 60;
    QColor defaultNodeColor;
    QColor highlightColor;

    void drawTree();
    void calculateNodePositions(const std::shared_ptr<BSTNode>& node, double x, double y, 
                              double width, std::map<int, NodeGraphics>& newItems);
    void animateNodes(const std::map<int, NodeGraphics>& newItems);
    void clearScene();
};

#endif // TREEVISUALIZER_H
