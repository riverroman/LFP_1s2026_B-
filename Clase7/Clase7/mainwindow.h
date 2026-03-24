#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnCargarArchivo_clicked();
    void on_btnAnalizar_clicked();
    void on_btnGenerarReportes_clicked();

private:
    Ui::MainWindow *ui;
    QString archivoActual;      // Nota: ===== ruta del archivo =====
    QString contenidoArchivo;   // Nota: ===== texto del archivo ====
};

#endif