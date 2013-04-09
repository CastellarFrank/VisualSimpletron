#ifndef SIMPLETRON_H
#define SIMPLETRON_H
#include <QString>
#include <QFile>
#include <QIODevice>
#include <QTextStream>
#include <cstdlib>

using namespace std;
class Simpletron
{
public:
    Simpletron(int cant=100);
    void clear_memory();
    void separar(QString palabra);
    void ingresar(QString palabra);
    bool leerArchivo(QString path);
    bool guardarInformacionEjecucion(QString path);
    bool guardarInstrucciones(QString path);
    bool verificarPalabra(QString palabra);
    bool validarInstruccion(int ins);
    void ingresarPalabra(QString palabra);
    void cargar();
    void almacenar();
    void sumar();
    void restar();
    void dividir();
    void multiplicar();
    void bifurcar();
    void bifurcarNegativo();
    void bifucarCero();
    void alto();
    QString obtenerPalabra();
    int ejecutarInstruccion();
    int ejecutarFuncion(int pos);
    int getCounter()const;
    int getInstruction()const;
    int getOperator()const;
    int getAcumulador()const;
    int getTotalInstrucciones()const;
    void reset();
    QString texto;
    QString infoMemoria;
    ~Simpletron();

private:
    QString *memory;
    int size_memory;
    int acumulador;
    int totalInstrucciones;
    int counter;
    int instruction;
    int operators;
    QString comando;


};

#endif // SIMPLETRON_H
