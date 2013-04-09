#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->listWidget,SIGNAL(itemClicked(QListWidgetItem*)),this,
            SLOT(modificarInformacion(QListWidgetItem*)));
    this->simpletron=new Simpletron();
    this->iniciarElementos();
    ui->txtMensajes->setReadOnly(true);
    ui->txtProcesamiento->setReadOnly(true);
    this->setWindowTitle("Simpletron - Industrias BuenRecord");
}

MainWindow::~MainWindow()
{
    delete simpletron;
    delete ui;
}

void MainWindow::iniciarElementos(){
    ui->radAddNormal->setChecked(true);
    this->on_radAddNormal_clicked();
    ui->txtPosicion->setText("0");
    ui->txtModificarPosicion->setText("0");
}

void MainWindow::modificarInformacion(QListWidgetItem *i){
    char temp[10];
    ui->txtModificarInstruccion->setText(i->text());
    ui->txtModificarPosicion->setText(itoa(ui->listWidget->currentRow(),temp,10));
}

void MainWindow::on_btnAddInstruction_clicked()
{
    if(ui->radAddNormal->isChecked()){
        if(ui->txtInstruction->text().isEmpty()){
            QMessageBox::warning(this,"Instrucción vacia","Debe definir una instrucción a ingresar.");
        }else{
            ui->listWidget->addItem(ui->txtInstruction->text());
            ui->txtInstruction->setText(0);
            ui->txtInstruction->setFocus();
        }
    }else{
        if(ui->txtInstruction->text().isEmpty() || ui->txtPosicion->text().isEmpty()){
            QMessageBox::warning(this,"Campos incompletos","Debe completar los campos de posición e instrucción");
        }else{
            ui->listWidget->insertItem(ui->txtPosicion->text().toInt(),
                                   ui->txtInstruction->text());
        }
    }
}

void MainWindow::on_btnRemoverInstruction_clicked()
{
    if(ui->listWidget->currentRow()!=-1){
        delete ui->listWidget->takeItem(ui->listWidget->currentRow());
    }else{
        QMessageBox::warning(this,"Instrucción no encontrada","Debe seleccionar la instrucción que desea eliminar.");
    }


}

void MainWindow::on_txtInstruction_returnPressed()
{
    this->on_btnAddInstruction_clicked();
}
bool MainWindow::verificarInstrucciones(){
    QString val;
    QString palabra;
    int ins=0;
    for(int i=0;i<ui->listWidget->count();i++){
        palabra=ui->listWidget->item(i)->text();
        QString palabra2=palabra;
        ins=palabra2.remove(0,1).toInt()/100;
        if(!this->simpletron->verificarPalabra(palabra)){
           val+="Error: ";
           val+=palabra;
           val+=" <--Formato inválido";
           val+="\n";
       }else if(!simpletron->validarInstruccion(ins)){
           val+="Error: ";
           val+=palabra;
           val+=" <--Instrucción desconocida";
           val+="\n";
       }
    }
    if(!val.isNull()){
        QMessageBox messagebox;
        messagebox.setWindowTitle("Instrucciones inválidas");
        messagebox.setText("Debe corregir las siguientes instrucciones inválidas");
        messagebox.setInformativeText("Presione Show Details para ver los errores");
        messagebox.setStandardButtons(QMessageBox::Ok);
        messagebox.setDetailedText(val);
        messagebox.setIcon(QMessageBox::Critical);
        messagebox.setDefaultButton(QMessageBox::Ok);
        messagebox.exec();
        return false;
    }else{
       return true;
    }
}


void MainWindow::on_btnVerificarInstrucciones_clicked()
{
    if(ui->listWidget->count()==0){
        QMessageBox::information(this,"Instrucciones no encontradas",
                                 "No hay ninguna instrucción ingresada");
    }else{
        if(this->verificarInstrucciones())
            QMessageBox::information(this,"Instrucciones Correctas",
                                     "No hay errores en las instrucciones");
    }
}


void MainWindow::on_radAddNormal_clicked()
{
    ui->radAddPos->setChecked(false);
    ui->lblAgregarPosicion->setVisible(false);
    ui->txtPosicion->setVisible(false);
}

void MainWindow::on_radAddPos_clicked()
{
    ui->radAddNormal->setChecked(false);
    ui->txtPosicion->setVisible(true);
    ui->lblAgregarPosicion->setVisible(true);
}


void MainWindow::on_btnModificarInstruccion_clicked()
{
    int val2=ui->txtModificarPosicion->text().toInt();
    if(ui->txtModificarPosicion->text().isEmpty() || ui->txtModificarInstruccion->text().isEmpty()){
        QMessageBox::warning(this,"Campos Incompletos",
                             "Debe completas los campos de posición e instrucción.");
    }else if(val2<ui->listWidget->count() && val2>=0){
        int val=ui->txtModificarPosicion->text().toInt();
        delete ui->listWidget->takeItem(val);
        ui->listWidget->insertItem(val,ui->txtModificarInstruccion->text());
    }else{
        QMessageBox::warning(this,"Posición Inválida",
                             "La posición especificada es inválida.");
    }

}

void MainWindow::on_pushButton_clicked()
{
    int valor=ui->listWidget->count();
    for(int i=0;i<valor;i++){
        delete ui->listWidget->takeItem(0);
    }
}

void MainWindow::on_btnDocumentTxt_clicked()
{
    QString filePath=QFileDialog::getOpenFileName(this,"Abrir Documento", QDir::currentPath(),"Documentos de texto (*.txt)");
    qDebug()<<filePath;
    if(!filePath.isNull()){
        this->on_pushButton_clicked();
        QFile file(filePath);
        if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
            QTextStream leer(&file);
            QString val;
            while(!leer.atEnd()){
                val=leer.readLine();
                ui->listWidget->addItem(val);
            }
            file.close();
        }

    }

}

void MainWindow::on_txtEjecutar_clicked()
{
    if(ui->listWidget->count()==0){
        QMessageBox::information(this,"Instrucciones no encontradas",
                                 "No hay ninguna instrucción ingresada");
    }else{
        if(this->verificarInstrucciones()){
            this->simpletron->reset();
            for(int i=0;i<ui->listWidget->count();i++){
                QString palabra=ui->listWidget->item(i)->text();
                simpletron->ingresar(palabra);
            }
            this->ejecutarPrograma();
        }
    }
}
void MainWindow::ejecutarPrograma(){
    for(int i=simpletron->getCounter();i<simpletron->getTotalInstrucciones();i=this->simpletron->getCounter()){
        int val=this->simpletron->ejecutarFuncion(i);
        if(val==1){
            bool ok;
            QString texto;
            while(ok==false || texto.isEmpty())
                texto=QInputDialog::getText(this,"Información requerida","Ingrese una palabra: ",QLineEdit::Normal,"",&ok,0);
            simpletron->ingresarPalabra(texto);
        }else if(val==2){
            QString texto=ui->txtMensajes->toPlainText();
            texto+=simpletron->obtenerPalabra();
            texto+="\n";
            ui->txtMensajes->setText(texto);
        }else if(val==3){
            break;
        }
    }
    ui->txtProcesamiento->setText(simpletron->texto);
    ui->txtMensajes->setText(simpletron->infoMemoria);
}

void MainWindow::on_btnGuardarComandos_clicked()
{
    if(ui->listWidget->count()!=0){
        if(this->verificarInstrucciones()){
            simpletron->clear_memory();
            for(int i=0;i<ui->listWidget->count();i++){
                QString palabra=ui->listWidget->item(i)->text();
                simpletron->ingresar(palabra);
            }
        }
        QString path=QFileDialog::getSaveFileName(this,"Guardar Instrucciones",
                                                  QDir::currentPath(),
                                                  "Documentos de texto (*.txt)");

        bool ok=simpletron->guardarInstrucciones(path);
        if(ok){
            QMessageBox::information(this,"Guardado completado",
                                     "La información se guardó exitosamente.");
        }else{
            QMessageBox::warning(this,"Error al guardar",
                                 "No se pudo completar la acción de guardado.");
        }
    }else{
        QMessageBox::warning(this,"Lista Vacia",
                             "No hay instrucciones a guardar.");
    }

}

void MainWindow::on_btnGuardarInfoEjecucion_clicked()
{
    if(!ui->txtProcesamiento->toPlainText().isEmpty()){
        QString path=QFileDialog::getSaveFileName(this,"Guardar Información",
                                                  QDir::currentPath(),
                                                  "Documentos de texto (*.txt)");
        bool ok=simpletron->guardarInformacionEjecucion(path);
        if(ok){
            QMessageBox::information(this,"Guardado completado",
                                     "La información se guardó exitosamente.");
        }else{
            QMessageBox::warning(this,"Error al guardar",
                                 "No se pudo completar la acción de guardado.");
        }
    }else{
        QMessageBox::warning(this,"Sin ejecución",
                             "Aún no ha ejecutado ningún programa.");
    }

}
