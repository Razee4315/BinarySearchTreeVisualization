#include "treevisualizer.h"
#include <QResizeEvent>
#include <QPen>
#include <QBrush>
#include <QVariantAnimation>

TreeVisualizer::TreeVisualizer(QWidget *parent)
    : QGraphicsView(parent)
    , scene(new QGraphicsScene(this))
    , defaultNodeColor(QColor(100, 181, 246))  // Material Blue 300
    , highlightColor(QColor(76, 175, 80))      // Material Green 500
{
    setScene(scene);
    setRenderHint(QPainter::Antialiasing);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setBackgroundBrush(QBrush(Qt::white));
}

TreeVisualizer::~TreeVisualizer() {
    clearScene();
}

void TreeVisualizer::clearScene() {
    for (auto& [value, graphics] : nodeItems) {
        delete graphics.circle;
        delete graphics.text;
        delete graphics.parentLine;
    }
    nodeItems.clear();
    scene->clear();
}

void TreeVisualizer::setBST(const std::shared_ptr<BinarySearchTree>& newBST) {
    bst = newBST;
    updateTree();
}

void TreeVisualizer::updateTree() {
    clearScene();
    if (!bst || !bst->getRoot()) {
        return;
    }
    drawTree();
}

void TreeVisualizer::drawTree() {
    std::map<int, NodeGraphics> newItems;
    double sceneWidth = width() - 2 * NODE_RADIUS;
    calculateNodePositions(bst->getRoot(), sceneWidth / 2, NODE_RADIUS + 10, sceneWidth / 4, newItems);
    
    for (auto& [value, graphics] : newItems) {
        if (!graphics.circle) {
            graphics.circle = new QGraphicsEllipseItem(-NODE_RADIUS, -NODE_RADIUS, 
                                                     2 * NODE_RADIUS, 2 * NODE_RADIUS);
            graphics.circle->setBrush(QBrush(defaultNodeColor));
            graphics.circle->setPen(QPen(Qt::black));
            scene->addItem(graphics.circle);
        }
        
        if (!graphics.text) {
            graphics.text = new QGraphicsTextItem(QString::number(value));
            graphics.text->setDefaultTextColor(Qt::black);
            scene->addItem(graphics.text);
        }
        
        graphics.circle->setPos(graphics.targetPos);
        graphics.text->setPos(graphics.targetPos.x() - 10, graphics.targetPos.y() - 10);
    }
    
    nodeItems = std::move(newItems);
}

void TreeVisualizer::calculateNodePositions(const std::shared_ptr<BSTNode>& node, double x, double y,
                                          double offset, std::map<int, NodeGraphics>& newItems) {
    if (!node) return;

    NodeGraphics& graphics = newItems[node->value];
    graphics.targetPos = QPointF(x, y);

    if (node->left) {
        calculateNodePositions(node->left, x - offset, y + LEVEL_HEIGHT, offset / 2, newItems);
        QPointF startPos(x, y);
        QPointF endPos = newItems[node->left->value].targetPos;
        auto* line = new QGraphicsLineItem(QLineF(startPos, endPos));
        scene->addItem(line);
        graphics.parentLine = line;
    }

    if (node->right) {
        calculateNodePositions(node->right, x + offset, y + LEVEL_HEIGHT, offset / 2, newItems);
        QPointF startPos(x, y);
        QPointF endPos = newItems[node->right->value].targetPos;
        auto* line = new QGraphicsLineItem(QLineF(startPos, endPos));
        scene->addItem(line);
        if (!graphics.parentLine) {
            graphics.parentLine = line;
        }
    }
}

void TreeVisualizer::highlightPath(const std::vector<int>& path, QColor color) {
    for (const auto& value : path) {
        auto it = nodeItems.find(value);
        if (it != nodeItems.end() && it->second.circle) {
            it->second.circle->setBrush(QBrush(color));
        }
    }
}

void TreeVisualizer::clearHighlights() {
    for (auto& [value, graphics] : nodeItems) {
        if (graphics.circle) {
            graphics.circle->setBrush(QBrush(defaultNodeColor));
        }
    }
}

void TreeVisualizer::resizeEvent(QResizeEvent* event) {
    QGraphicsView::resizeEvent(event);
    scene->setSceneRect(0, 0, event->size().width(), event->size().height());
    if (!nodeItems.empty()) {
        updateTree();
    }
}
