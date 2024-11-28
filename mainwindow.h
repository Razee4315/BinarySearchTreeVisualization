#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QSpinBox>
#include <QComboBox>
#include "treevisualizer.h"
#include <memory>
#include "binarysearchtree.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void handleInsert();
    void handleDelete();
    void handleSearch();
    void handleClear();
    void handleRandomInsert();
    void handleTraversal();
    void handleSaveTree();
    void handleLoadTree();
    void handleZoomIn();
    void handleZoomOut();
    void handleResetZoom();

private:
    void setupUI();
    void createMenuBar();
    void createToolBar();
    void createStatusBar();
    void loadSettings();
    void saveSettings();
    QString traversalToString(const std::vector<int>& nodes);
    QPushButton* createStyledButton(const QString& text, const QString& color);
    QString adjustColor(const QString& color, double factor);
    void showBSTGuide();
    void validateBST();

    std::shared_ptr<BinarySearchTree> bst;
    TreeVisualizer* treeVisualizer;
    QLineEdit* inputField;
    QPushButton* insertButton;
    QPushButton* deleteButton;
    QPushButton* searchButton;
    QPushButton* clearButton;
    QPushButton* randomButton;
    QComboBox* traversalCombo;
    QPushButton* traversalButton;
    QSpinBox* randomCountSpinner;
    QLabel* statusLabel;
    QLabel* logoLabel;
    double currentZoom;
};

#endif // MAINWINDOW_H
