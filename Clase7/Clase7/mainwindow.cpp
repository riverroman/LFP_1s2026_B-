#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>
#include <fstream>
#include <sstream>

// ===== Clases ===========
#include "Lexicalanalyzer.h"
#include "Reportgenerator.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// ── Botón: Cargar Archivo ──────────────────────────────────────
void MainWindow::on_btnCargarArchivo_clicked()
{
    QString ruta = QFileDialog::getOpenFileName(
        this,
        "Seleccionar archivo de entrada",
        "",
        "Archivos de texto (*.txt);;Todos los archivos (*)"
        );

    if (ruta.isEmpty()) return;

    archivoActual = ruta;

    // Leer el archivo
    std::ifstream file(ruta.toStdString());
    if (!file.is_open()) {
        QMessageBox::critical(this, "Error", "No se pudo abrir el archivo.");
        return;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    contenidoArchivo = QString::fromStdString(buffer.str());

    // Mostrar en el área de texto
    ui->textArea->setText(contenidoArchivo);
    ui->textArea->setStyleSheet("color: white; background-color: black;");

    setWindowTitle("Proyecto 1 - LFP  |  " + ruta);
}

// ── Botón: Analizar ────────────────────────────────────────────
void MainWindow::on_btnAnalizar_clicked()
{
    if (contenidoArchivo.isEmpty()) {
        QMessageBox::warning(this, "Aviso", "Primero carga un archivo.");
        return;
    }

    std::string input = contenidoArchivo.toStdString();
    LexicalAnalyzer lexer(input);
    lexer.analyze();

    QString resultado = "=== TOKENS ENCONTRADOS ===\n\n";
    for (auto& token : lexer.tokens) {
        resultado += QString("[%1] → \"%2\"  (línea %3, col %4)\n")
                         .arg(QString::fromStdString(token.getTypeString()))
                         .arg(QString::fromStdString(token.lexeme))
                         .arg(token.line)
                         .arg(token.column);
    }

    if (!lexer.errors.empty()) {
        resultado += "\n=== ERRORES LÉXICOS ===\n\n";
        for (auto& err : lexer.errors) {
            resultado += QString("⚠ \"%1\" → %2  (línea %3, col %4)\n")
                             .arg(QString::fromStdString(err.lexeme))
                             .arg(QString::fromStdString(err.description))
                             .arg(err.line)
                             .arg(err.column);
        }
    }

    ui->textArea->setText(resultado);
}

// ── Botón: Generar Reportes ────────────────────────────────────
void MainWindow::on_btnGenerarReportes_clicked()
{
    if (contenidoArchivo.isEmpty()) {
        QMessageBox::warning(this, "Aviso", "Primero carga un archivo.");
        return;
    }
    std::string input = contenidoArchivo.toStdString();
    LexicalAnalyzer lexer(input);
    lexer.analyze();

    ReportGenerator::generateTokenReport(lexer.tokens, "token_report.html");
    ReportGenerator::generateErrorReport(lexer.errors, "error_report.html");
    ReportGenerator::generateStatisticsReport(lexer.tokens, "statistics_report.html"); // ← nuevo

    QDesktopServices::openUrl(QUrl::fromLocalFile(QDir::currentPath() + "/token_report.html"));
    QDesktopServices::openUrl(QUrl::fromLocalFile(QDir::currentPath() + "/error_report.html"));
    QDesktopServices::openUrl(QUrl::fromLocalFile(QDir::currentPath() + "/statistics_report.html")); // ← nuevo

    QMessageBox::information(this, "Listo", "3 reportes generados y abiertos en el navegador.");
}
