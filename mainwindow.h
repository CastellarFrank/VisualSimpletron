#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QList>
#include "simpletron.h"
#include <QListWidgetItem>
#include <QInputDialog>
#include <QDebug>
#include <QFileDialog>
#include <QTextStream>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void iniciarElementos();
    bool verificarInstrucciones();
    void ejecutarPrograma();
    Simpletron *simpletron;

    Ui::MainWindow *ui;


private slots:
    void on_btnGuardarInfoEjecucion_clicked();
    void on_btnGuardarComandos_clicked();
    void on_txtEjecutar_clicked();
    void on_btnDocumentTxt_clicked();
    void on_pushButton_clicked();
    void on_btnModificarInstruccion_clicked();
    void on_radAddPos_clicked();
    void on_radAddNormal_clicked();
    void on_btnVerificarInstrucciones_clicked();
    void on_txtInstruction_returnPressed();
    void on_btnRemoverInstruction_clicked();
    void on_btnAddInstruction_clicked();
    void modificarInformacion(QListWidgetItem *i);
};

#endif // MAINWINDOW_H
