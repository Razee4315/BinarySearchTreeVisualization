#include "mainwindow.h"

#include <QApplication>
#include <QStyle>
#include <QScreen>
#include <QPalette>
#include <QStyleFactory>
#include <QFont>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    // Force light mode with Fusion style for consistency
    qApp->setStyle(QStyleFactory::create("Fusion"));
    
    // Set default font for better readability
    QFont defaultFont("Segoe UI", 9);  // Windows default font, size 9
    defaultFont.setStyleStrategy(QFont::PreferAntialias);
    qApp->setFont(defaultFont);
    
    // Create and customize the light palette
    QPalette lightPalette;
    
    // Main colors - using slightly warmer whites and pure black for better contrast
    lightPalette.setColor(QPalette::Window, QColor(248, 248, 250));        // Slightly blue-tinted white
    lightPalette.setColor(QPalette::WindowText, QColor(0, 0, 0));          // Pure black
    lightPalette.setColor(QPalette::Base, QColor(255, 255, 255));          // Pure white
    lightPalette.setColor(QPalette::AlternateBase, QColor(248, 248, 250)); // Matching window color
    
    // Text colors - pure black for maximum readability
    lightPalette.setColor(QPalette::Text, QColor(0, 0, 0));               // Pure black
    lightPalette.setColor(QPalette::ToolTipText, QColor(0, 0, 0));        // Pure black
    
    // Button colors - slightly darker for better visibility
    lightPalette.setColor(QPalette::Button, QColor(230, 230, 235));       // Slightly darker than window
    lightPalette.setColor(QPalette::ButtonText, QColor(0, 0, 0));         // Pure black
    
    // Highlight colors - more vibrant blue
    lightPalette.setColor(QPalette::Highlight, QColor(0, 120, 215));      // Windows blue
    lightPalette.setColor(QPalette::HighlightedText, QColor(255, 255, 255)); // White
    
    // Link color - standard blue
    lightPalette.setColor(QPalette::Link, QColor(0, 102, 204));          // Standard link blue
    
    // Disabled colors - using a darker gray for better visibility
    lightPalette.setColor(QPalette::Disabled, QPalette::WindowText, QColor(150, 150, 150));
    lightPalette.setColor(QPalette::Disabled, QPalette::Text, QColor(150, 150, 150));
    lightPalette.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(150, 150, 150));
    
    // Tooltip colors
    lightPalette.setColor(QPalette::ToolTipBase, QColor(255, 255, 220)); // Light yellow for tooltips
    
    // Apply the palette
    qApp->setPalette(lightPalette);
    
    // Disable platform theme integration
    qApp->setProperty("QT_USE_NATIVE_WINDOWS", false);
    qApp->setProperty("QT_USE_NATIVE_MENUS", false);
    
    // Set stylesheet for better button appearance
    qApp->setStyleSheet(R"(
        QPushButton {
            padding: 5px 10px;
            border: 1px solid #b1b1b1;
            border-radius: 3px;
            background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                          stop:0 #f8f8f8, stop:1 #e1e1e1);
        }
        QPushButton:hover {
            background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                          stop:0 #f0f0f0, stop:1 #d7d7d7);
        }
        QPushButton:pressed {
            background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                          stop:0 #d7d7d7, stop:1 #f0f0f0);
        }
        QLineEdit {
            padding: 3px;
            border: 1px solid #b1b1b1;
            border-radius: 2px;
            background-color: white;
        }
        QLineEdit:focus {
            border: 1px solid #0078d7;
        }
    )");
    
    MainWindow w;
    
    // Center the window on the screen
    const QRect screenGeometry = w.screen()->geometry();
    const QRect windowGeometry = w.frameGeometry();
    w.move(screenGeometry.center() - windowGeometry.center());
    
    w.show();
    return a.exec();
}
