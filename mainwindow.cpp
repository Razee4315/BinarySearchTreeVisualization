#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QIntValidator>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QFileDialog>
#include <QSettings>
#include <QRandomGenerator>
#include <QStyle>
#include <QApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , bst(std::make_shared<BinarySearchTree>())
    , currentZoom(1.0)
{
    setupUI();
    createMenuBar();
    createToolBar();
    createStatusBar();
    loadSettings();
}

void MainWindow::setupUI() {
    auto* centralWidget = new QWidget;
    auto* mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    // Header with logo and title
    auto* headerWidget = new QWidget;
    auto* headerLayout = new QHBoxLayout(headerWidget);
    headerLayout->setSpacing(15);

    logoLabel = new QLabel;
    QPixmap logo(":/resources/logo.svg");
    logoLabel->setPixmap(logo.scaled(40, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    
    auto* titleLabel = new QLabel("Binary Search Tree Visualizer");
    titleLabel->setStyleSheet(
        "font-size: 24px;"
        "font-weight: bold;"
        "color: #2196F3;"
    );
    
    headerLayout->addWidget(logoLabel);
    headerLayout->addWidget(titleLabel);
    headerLayout->addStretch();
    
    // Main controls panel
    auto* controlsPanel = new QWidget;
    controlsPanel->setStyleSheet(
        "QWidget {"
        "    background: #f8f9fa;"
        "    border-radius: 10px;"
        "    padding: 10px;"
        "}"
    );
    auto* controlsLayout = new QHBoxLayout(controlsPanel);
    controlsLayout->setSpacing(15);

    // Input section
    inputField = new QLineEdit;
    inputField->setPlaceholderText("Enter a number...");
    inputField->setStyleSheet(
        "QLineEdit {"
        "    padding: 10px;"
        "    border: 2px solid #e0e0e0;"
        "    border-radius: 5px;"
        "    background: white;"
        "    font-size: 14px;"
        "    min-width: 150px;"
        "}"
        "QLineEdit:focus {"
        "    border-color: #2196F3;"
        "}"
    );

    // Basic operation buttons
    insertButton = createStyledButton("Insert", "#4CAF50");
    deleteButton = createStyledButton("Delete", "#F44336");
    searchButton = createStyledButton("Search", "#2196F3");
    clearButton = createStyledButton("Clear Tree", "#757575");

    controlsLayout->addWidget(inputField);
    controlsLayout->addWidget(insertButton);
    controlsLayout->addWidget(deleteButton);
    controlsLayout->addWidget(searchButton);
    controlsLayout->addWidget(clearButton);

    // Advanced controls panel
    auto* advancedPanel = new QWidget;
    advancedPanel->setStyleSheet(
        "QWidget {"
        "    background: #f8f9fa;"
        "    border-radius: 10px;"
        "    padding: 10px;"
        "}"
    );
    auto* advancedLayout = new QHBoxLayout(advancedPanel);
    advancedLayout->setSpacing(15);

    // Random insertion controls
    auto* randomGroup = new QWidget;
    auto* randomLayout = new QHBoxLayout(randomGroup);
    randomLayout->setSpacing(10);
    
    randomButton = createStyledButton("Random Insert", "#9C27B0");
    
    randomCountSpinner = new QSpinBox;
    randomCountSpinner->setRange(1, 100);
    randomCountSpinner->setValue(5);
    randomCountSpinner->setPrefix("Count: ");
    randomCountSpinner->setStyleSheet(
        "QSpinBox {"
        "    padding: 10px;"
        "    border: 2px solid #e0e0e0;"
        "    border-radius: 5px;"
        "    background: white;"
        "    font-size: 14px;"
        "    min-width: 120px;"
        "}"
        "QSpinBox:focus {"
        "    border-color: #2196F3;"
        "}"
    );

    randomLayout->addWidget(randomButton);
    randomLayout->addWidget(randomCountSpinner);

    // Traversal controls
    auto* traversalGroup = new QWidget;
    auto* traversalLayout = new QHBoxLayout(traversalGroup);
    traversalLayout->setSpacing(10);

    traversalCombo = new QComboBox;
    traversalCombo->addItems({"Inorder", "Preorder", "Postorder"});
    traversalCombo->setStyleSheet(
        "QComboBox {"
        "    padding: 10px;"
        "    border: 2px solid #e0e0e0;"
        "    border-radius: 5px;"
        "    background: white;"
        "    font-size: 14px;"
        "    min-width: 150px;"
        "}"
        "QComboBox:focus {"
        "    border-color: #2196F3;"
        "}"
    );
    
    traversalButton = createStyledButton("Show Traversal", "#673AB7");

    traversalLayout->addWidget(traversalCombo);
    traversalLayout->addWidget(traversalButton);

    // Educational tools
    auto* helpButton = createStyledButton("BST Guide", "#FF5722");
    connect(helpButton, &QPushButton::clicked, this, &MainWindow::showBSTGuide);
    
    auto* validateButton = createStyledButton("Validate BST", "#607D8B");
    connect(validateButton, &QPushButton::clicked, this, &MainWindow::validateBST);

    advancedLayout->addWidget(randomGroup);
    advancedLayout->addWidget(traversalGroup);
    advancedLayout->addWidget(helpButton);
    advancedLayout->addWidget(validateButton);

    // Tree visualizer
    treeVisualizer = new TreeVisualizer(this);
    treeVisualizer->setBST(bst);
    treeVisualizer->setMinimumHeight(400);
    treeVisualizer->setStyleSheet(
        "QWidget {"
        "    background: white;"
        "    border: 2px solid #e0e0e0;"
        "    border-radius: 10px;"
        "}"
    );

    // Status bar
    statusLabel = new QLabel("Ready");
    statusLabel->setStyleSheet(
        "QLabel {"
        "    color: #666666;"
        "    font-size: 13px;"
        "}"
    );

    // Add all components to main layout
    mainLayout->addWidget(headerWidget);
    mainLayout->addWidget(controlsPanel);
    mainLayout->addWidget(advancedPanel);
    mainLayout->addWidget(treeVisualizer, 1);
    mainLayout->addWidget(statusLabel);

    setCentralWidget(centralWidget);

    // Connect signals
    connect(inputField, &QLineEdit::returnPressed, this, &MainWindow::handleInsert);
    connect(insertButton, &QPushButton::clicked, this, &MainWindow::handleInsert);
    connect(deleteButton, &QPushButton::clicked, this, &MainWindow::handleDelete);
    connect(searchButton, &QPushButton::clicked, this, &MainWindow::handleSearch);
    connect(clearButton, &QPushButton::clicked, this, &MainWindow::handleClear);
    connect(randomButton, &QPushButton::clicked, this, &MainWindow::handleRandomInsert);
    connect(traversalButton, &QPushButton::clicked, this, &MainWindow::handleTraversal);

    // Set window properties
    resize(1200, 800);
    setWindowTitle("BST Visualizer");
    setWindowIcon(QIcon(":/resources/logo.svg"));
}

QPushButton* MainWindow::createStyledButton(const QString& text, const QString& color) {
    auto* button = new QPushButton(text);
    button->setStyleSheet(QString(
        "QPushButton {"
        "    background-color: %1;"
        "    color: white;"
        "    border: none;"
        "    border-radius: 5px;"
        "    padding: 10px 20px;"
        "    font-size: 14px;"
        "    font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "    background-color: %2;"
        "}"
        "QPushButton:pressed {"
        "    background-color: %3;"
        "}"
    ).arg(color).arg(adjustColor(color, 1.1)).arg(adjustColor(color, 0.9)));
    button->setCursor(Qt::PointingHandCursor);
    return button;
}

QString MainWindow::adjustColor(const QString& color, double factor) {
    QColor c(color);
    int h, s, v;
    c.getHsv(&h, &s, &v);
    v = qBound(0, static_cast<int>(v * factor), 255);
    c.setHsv(h, s, v);
    return c.name();
}

void MainWindow::showBSTGuide() {
    QDialog* guideDialog = new QDialog(this);
    guideDialog->setWindowTitle("BST Guide");
    guideDialog->setMinimumSize(600, 400);
    
    auto* layout = new QVBoxLayout(guideDialog);
    auto* tabs = new QTabWidget;
    
    // Overview Tab
    auto* overviewTab = new QWidget;
    auto* overviewLayout = new QVBoxLayout(overviewTab);
    auto* overviewText = new QLabel(
        "<h2>Binary Search Tree Overview</h2>"
        "<p>A Binary Search Tree (BST) is a binary tree where for each node:</p>"
        "<ul>"
        "<li>All nodes in the left subtree have values less than the node's value</li>"
        "<li>All nodes in the right subtree have values greater than the node's value</li>"
        "</ul>"
        "<p>This property makes BSTs efficient for searching, inserting, and deleting elements.</p>"
        "<h3>Time Complexity:</h3>"
        "<ul>"
        "<li>Search: O(log n) average, O(n) worst</li>"
        "<li>Insert: O(log n) average, O(n) worst</li>"
        "<li>Delete: O(log n) average, O(n) worst</li>"
        "</ul>"
    );
    overviewText->setWordWrap(true);
    overviewLayout->addWidget(overviewText);
    tabs->addTab(overviewTab, "Overview");
    
    // Operations Tab
    auto* operationsTab = new QWidget;
    auto* operationsLayout = new QVBoxLayout(operationsTab);
    auto* operationsText = new QLabel(
        "<h2>BST Operations</h2>"
        "<h3>Insertion:</h3>"
        "<ol>"
        "<li>Start at the root</li>"
        "<li>If value < node: go left</li>"
        "<li>If value > node: go right</li>"
        "<li>If empty spot found: insert new node</li>"
        "</ol>"
        "<h3>Deletion:</h3>"
        "<ol>"
        "<li>Find node to delete</li>"
        "<li>If leaf node: simply remove</li>"
        "<li>If one child: replace with child</li>"
        "<li>If two children: replace with inorder successor</li>"
        "</ol>"
        "<h3>Search:</h3>"
        "<ol>"
        "<li>Start at root</li>"
        "<li>If value found: return true</li>"
        "<li>If value < node: search left subtree</li>"
        "<li>If value > node: search right subtree</li>"
        "</ol>"
    );
    operationsText->setWordWrap(true);
    operationsLayout->addWidget(operationsText);
    tabs->addTab(operationsTab, "Operations");
    
    // Traversals Tab
    auto* traversalsTab = new QWidget;
    auto* traversalsLayout = new QVBoxLayout(traversalsTab);
    auto* traversalsText = new QLabel(
        "<h2>Tree Traversals</h2>"
        "<h3>Inorder (Left-Root-Right):</h3>"
        "<ul>"
        "<li>Visit left subtree</li>"
        "<li>Visit root</li>"
        "<li>Visit right subtree</li>"
        "<li>Result: Sorted sequence for BST</li>"
        "</ul>"
        "<h3>Preorder (Root-Left-Right):</h3>"
        "<ul>"
        "<li>Visit root</li>"
        "<li>Visit left subtree</li>"
        "<li>Visit right subtree</li>"
        "<li>Use: Creating a copy of the tree</li>"
        "</ul>"
        "<h3>Postorder (Left-Right-Root):</h3>"
        "<ul>"
        "<li>Visit left subtree</li>"
        "<li>Visit right subtree</li>"
        "<li>Visit root</li>"
        "<li>Use: Deleting the tree</li>"
        "</ul>"
    );
    traversalsText->setWordWrap(true);
    traversalsLayout->addWidget(traversalsText);
    tabs->addTab(traversalsTab, "Traversals");
    
    layout->addWidget(tabs);
    
    auto* closeButton = new QPushButton("Close");
    connect(closeButton, &QPushButton::clicked, guideDialog, &QDialog::accept);
    layout->addWidget(closeButton);
    
    guideDialog->exec();
}

void MainWindow::createMenuBar() {
    auto* fileMenu = menuBar()->addMenu("&File");
    
    auto* saveAction = new QAction("&Save Tree", this);
    saveAction->setShortcut(QKeySequence::Save);
    connect(saveAction, &QAction::triggered, this, &MainWindow::handleSaveTree);
    fileMenu->addAction(saveAction);
    
    auto* loadAction = new QAction("&Load Tree", this);
    loadAction->setShortcut(QKeySequence::Open);
    connect(loadAction, &QAction::triggered, this, &MainWindow::handleLoadTree);
    fileMenu->addAction(loadAction);
    
    fileMenu->addSeparator();
    
    auto* exitAction = new QAction("E&xit", this);
    exitAction->setShortcut(QKeySequence::Quit);
    connect(exitAction, &QAction::triggered, this, &QWidget::close);
    fileMenu->addAction(exitAction);
    
    auto* viewMenu = menuBar()->addMenu("&View");
    
    auto* zoomInAction = new QAction("Zoom &In", this);
    zoomInAction->setShortcut(QKeySequence::ZoomIn);
    connect(zoomInAction, &QAction::triggered, this, &MainWindow::handleZoomIn);
    viewMenu->addAction(zoomInAction);
    
    auto* zoomOutAction = new QAction("Zoom &Out", this);
    zoomOutAction->setShortcut(QKeySequence::ZoomOut);
    connect(zoomOutAction, &QAction::triggered, this, &MainWindow::handleZoomOut);
    viewMenu->addAction(zoomOutAction);
    
    auto* resetZoomAction = new QAction("&Reset Zoom", this);
    connect(resetZoomAction, &QAction::triggered, this, &MainWindow::handleResetZoom);
    viewMenu->addAction(resetZoomAction);
    
    auto* helpMenu = menuBar()->addMenu("&Help");
    auto* aboutAction = new QAction("&About", this);
    connect(aboutAction, &QAction::triggered, this, [this]() {
        QMessageBox::about(this, "About BST Visualizer",
            "Binary Search Tree Visualizer\n\n"
            "An educational tool for visualizing BST operations.\n"
            "Version 1.0\n\n"
            "Created by Saqlain\n"
            "GitHub: github.com/Razee4315\n\n"
            " 2024 All rights reserved");
    });
    helpMenu->addAction(aboutAction);
}

void MainWindow::createToolBar() {
    auto* toolbar = addToolBar("Main Toolbar");
    toolbar->setMovable(false);
    
    toolbar->addAction(style()->standardIcon(QStyle::SP_FileIcon), "Save Tree", 
                      this, &MainWindow::handleSaveTree);
    toolbar->addAction(style()->standardIcon(QStyle::SP_DialogOpenButton), "Load Tree", 
                      this, &MainWindow::handleLoadTree);
    toolbar->addSeparator();
    toolbar->addAction(style()->standardIcon(QStyle::SP_ArrowUp), "Zoom In", 
                      this, &MainWindow::handleZoomIn);
    toolbar->addAction(style()->standardIcon(QStyle::SP_ArrowDown), "Zoom Out", 
                      this, &MainWindow::handleZoomOut);
}

void MainWindow::createStatusBar() {
    statusBar()->addPermanentWidget(statusLabel);
}

void MainWindow::handleInsert() {
    bool ok;
    int value = inputField->text().toInt(&ok);
    
    if (!ok || inputField->text().isEmpty()) {
        QMessageBox::warning(this, "Invalid Input", "Please enter a valid integer.");
        return;
    }
    
    try {
        if (bst->insert(value)) {
            statusLabel->setText(QString("Inserted %1").arg(value));
            treeVisualizer->updateTree();
        } else {
            statusLabel->setText(QString("%1 already exists in the tree").arg(value));
        }
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", QString("Failed to insert: %1").arg(e.what()));
    }
    
    inputField->clear();
}

void MainWindow::handleDelete() {
    bool ok;
    int value = inputField->text().toInt(&ok);
    
    if (!ok || inputField->text().isEmpty()) {
        QMessageBox::warning(this, "Invalid Input", "Please enter a valid integer.");
        return;
    }
    
    try {
        if (bst->remove(value)) {
            statusLabel->setText(QString("Deleted %1").arg(value));
            treeVisualizer->updateTree();
        } else {
            statusLabel->setText(QString("%1 not found in the tree").arg(value));
        }
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", QString("Failed to delete: %1").arg(e.what()));
    }
    
    inputField->clear();
}

void MainWindow::handleSearch() {
    bool ok;
    int value = inputField->text().toInt(&ok);
    
    if (!ok || inputField->text().isEmpty()) {
        QMessageBox::warning(this, "Invalid Input", "Please enter a valid integer.");
        return;
    }
    
    try {
        auto path = bst->search(value);
        if (!path.empty() && path.back() == value) {
            statusLabel->setText(QString("Found %1").arg(value));
            treeVisualizer->highlightPath(path, QColor(76, 175, 80)); // Material Green 500
        } else {
            statusLabel->setText(QString("%1 not found in the tree").arg(value));
            treeVisualizer->highlightPath(path, QColor(244, 67, 54)); // Material Red 500
        }
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", QString("Failed to search: %1").arg(e.what()));
    }
    
    inputField->clear();
}

void MainWindow::handleClear() {
    try {
        bst->clear();
        treeVisualizer->updateTree();
        statusLabel->setText("Tree cleared");
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", QString("Failed to clear tree: %1").arg(e.what()));
    }
    
    inputField->clear();
}

void MainWindow::handleRandomInsert() {
    int count = randomCountSpinner->value();
    int inserted = 0;
    
    for (int i = 0; i < count; ++i) {
        int value = QRandomGenerator::global()->bounded(1, 100);
        if (bst->insert(value)) {
            inserted++;
        }
    }
    
    treeVisualizer->updateTree();
    statusLabel->setText(QString("Inserted %1 random nodes").arg(inserted));
}

void MainWindow::handleTraversal() {
    if (bst->isEmpty()) {
        statusLabel->setText("Tree is empty");
        return;
    }
    
    std::vector<int> traversal;
    QString type = traversalCombo->currentText();
    
    if (type == "Inorder") {
        traversal = bst->inorderTraversal();
    } else if (type == "Preorder") {
        traversal = bst->preorderTraversal();
    } else if (type == "Postorder") {
        traversal = bst->postorderTraversal();
    }
    
    if (traversal.empty()) {
        statusLabel->setText("No nodes in traversal");
    } else {
        QString traversalStr = traversalToString(traversal);
        statusLabel->setText(QString("%1 Traversal: %2").arg(type).arg(traversalStr));
        
        // Clear previous highlights
        treeVisualizer->clearHighlights();
        
        // Highlight the traversal path with different colors for each type
        QColor highlightColor;
        if (type == "Inorder") {
            highlightColor = QColor("#4CAF50");  // Green
        } else if (type == "Preorder") {
            highlightColor = QColor("#2196F3");  // Blue
        } else {
            highlightColor = QColor("#FF9800");  // Orange
        }
        
        treeVisualizer->highlightPath(traversal, highlightColor);
        treeVisualizer->update();
    }
}

void MainWindow::handleSaveTree() {
    QString fileName = QFileDialog::getSaveFileName(this, "Save Tree", "", "Tree Files (*.tree)");
    if (fileName.isEmpty()) return;
    
    QSettings settings(fileName, QSettings::IniFormat);
    auto nodes = bst->serialize();
    
    settings.beginWriteArray("nodes");
    for (size_t i = 0; i < nodes.size(); ++i) {
        settings.setArrayIndex(static_cast<int>(i));
        settings.setValue("value", nodes[i]);
    }
    settings.endArray();
    
    statusLabel->setText("Tree saved successfully");
}

void MainWindow::handleLoadTree() {
    QString fileName = QFileDialog::getOpenFileName(this, "Load Tree", "", "Tree Files (*.tree)");
    if (fileName.isEmpty()) return;
    
    QSettings settings(fileName, QSettings::IniFormat);
    std::vector<int> nodes;
    
    int size = settings.beginReadArray("nodes");
    for (int i = 0; i < size; ++i) {
        settings.setArrayIndex(i);
        nodes.push_back(settings.value("value").toInt());
    }
    settings.endArray();
    
    bst->deserialize(nodes);
    treeVisualizer->updateTree();
    statusLabel->setText("Tree loaded successfully");
}

void MainWindow::handleZoomIn() {
    currentZoom *= 1.2;
    treeVisualizer->scale(1.2, 1.2);
}

void MainWindow::handleZoomOut() {
    currentZoom /= 1.2;
    treeVisualizer->scale(1/1.2, 1/1.2);
}

void MainWindow::handleResetZoom() {
    treeVisualizer->resetTransform();
    currentZoom = 1.0;
}

void MainWindow::loadSettings() {
    QSettings settings;
    restoreGeometry(settings.value("geometry").toByteArray());
}

void MainWindow::saveSettings() {
    QSettings settings;
    settings.setValue("geometry", saveGeometry());
}

QString MainWindow::traversalToString(const std::vector<int>& nodes) {
    QString result;
    for (size_t i = 0; i < nodes.size(); ++i) {
        result += QString::number(nodes[i]);
        if (i < nodes.size() - 1) {
            result += " → ";
        }
    }
    return result;
}

void MainWindow::validateBST() {
    if (bst->isEmpty()) {
        QMessageBox::information(this, "BST Validation", "Tree is empty");
        return;
    }
    
    bool isValid = true;
    QString message = "BST Properties:\n\n";
    
    // Check BST property
    std::function<bool(std::shared_ptr<BSTNode>, int, int)> isBST = 
        [&](std::shared_ptr<BSTNode> node, int min, int max) -> bool {
            if (!node) return true;
            
            if (node->value <= min || node->value >= max)
                return false;
            
            return isBST(node->left, min, node->value) &&
                   isBST(node->right, node->value, max);
        };
    
    isValid = isBST(bst->getRoot(), INT_MIN, INT_MAX);
    
    // Calculate properties
    std::function<int(std::shared_ptr<BSTNode>)> getHeight = 
        [&](std::shared_ptr<BSTNode> node) -> int {
            if (!node) return 0;
            return 1 + std::max(getHeight(node->left), getHeight(node->right));
        };
    
    std::function<int(std::shared_ptr<BSTNode>)> getSize = 
        [&](std::shared_ptr<BSTNode> node) -> int {
            if (!node) return 0;
            return 1 + getSize(node->left) + getSize(node->right);
        };
    
    int height = getHeight(bst->getRoot());
    int size = getSize(bst->getRoot());
    int minHeight = static_cast<int>(std::floor(std::log2(size + 1)));
    int maxHeight = size;
    
    message += QString("• BST Property: %1\n").arg(isValid ? "Valid ✓" : "Invalid ✗");
    message += QString("• Height: %1\n").arg(height);
    message += QString("• Number of nodes: %1\n").arg(size);
    message += QString("• Minimum possible height: %1\n").arg(minHeight);
    message += QString("• Maximum possible height: %1\n").arg(maxHeight);
    message += QString("• Balance factor: %1\n").arg(maxHeight - height);
    
    QMessageBox::information(this, "BST Validation", message);
}
