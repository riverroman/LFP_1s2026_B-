/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.11.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QPushButton *btnCargarArchivo;
    QTextEdit *textArea;
    QPushButton *btnGenerarReportes;
    QLabel *label;
    QPushButton *btnAnalizar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        btnCargarArchivo = new QPushButton(centralwidget);
        btnCargarArchivo->setObjectName("btnCargarArchivo");
        btnCargarArchivo->setGeometry(QRect(40, 50, 171, 51));
        textArea = new QTextEdit(centralwidget);
        textArea->setObjectName("textArea");
        textArea->setGeometry(QRect(40, 120, 731, 391));
        btnGenerarReportes = new QPushButton(centralwidget);
        btnGenerarReportes->setObjectName("btnGenerarReportes");
        btnGenerarReportes->setGeometry(QRect(240, 50, 171, 51));
        label = new QLabel(centralwidget);
        label->setObjectName("label");
        label->setGeometry(QRect(350, 0, 151, 41));
        btnAnalizar = new QPushButton(centralwidget);
        btnAnalizar->setObjectName("btnAnalizar");
        btnAnalizar->setGeometry(QRect(620, 50, 151, 51));
        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        btnCargarArchivo->setText(QCoreApplication::translate("MainWindow", "Cargar Archivo", nullptr));
        btnGenerarReportes->setText(QCoreApplication::translate("MainWindow", "Generar Reportes", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Proyecto 1 - LFP", nullptr));
        btnAnalizar->setText(QCoreApplication::translate("MainWindow", "Analizar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
